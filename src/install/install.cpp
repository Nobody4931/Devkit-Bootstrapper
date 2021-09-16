#define WIN32_LEAN_AND_MEAN

#include "install/install.hpp"

#include <windows.h>
#include <winreg.h>
#include <winuser.h>

#include <filesystem>
#include <iostream>
#include <fstream>
#include <regex>
#include <string>

#include "config.hpp"
#include "util/sfolders.hpp"

namespace fs = std::filesystem;


static const std::regex::flag_type re_flags = std::regex::flag_type::ECMAScript | std::regex::flag_type::optimize;

static const std::regex re_macro_user_dir( "BOOTSTRAP_USER_DIR", re_flags );
static const std::regex re_macro_binary_dir( "BOOTSTRAP_BINARY_DIR", re_flags );
static const std::regex re_macro_config_dir( "BOOTSTRAP_CONFIG_DIR", re_flags );


static void expand_macros_recursive( const fs::path& directory, const fs::path& user_dir, const fs::path& inst_dir, const fs::path& conf_dir ) {
	for ( const fs::directory_entry& entry : fs::directory_iterator( directory ) ) {
		if ( entry.is_regular_file() ) {
			std::ifstream file_stream( entry.path().string(), std::ifstream::in | std::ifstream::ate );
			if ( file_stream ) {
				std::string file_content( file_stream.tellg(), 0 );
				file_stream.seekg( 0, std::ifstream::beg );
				file_stream.read( &file_content[0], file_content.size() );
				file_stream.close();

				file_content = std::regex_replace( file_content, re_macro_user_dir, user_dir.string() );
				file_content = std::regex_replace( file_content, re_macro_binary_dir, inst_dir.string() );
				file_content = std::regex_replace( file_content, re_macro_config_dir, conf_dir.string() );

				std::ofstream file_output( entry.path().string(), std::ofstream::out | std::ofstream::trunc );
				if ( file_output ) {
					file_output << file_content.c_str(); // fuck you null characters
					file_output.close();
				}
			} // not doing error handling bc fuck it
		} else if ( entry.is_directory() ) {
			expand_macros_recursive( entry.path(), user_dir, inst_dir, conf_dir );
		}
	}
}


int program_install( int argc, char** argv ) {
	// Get directories to install
	fs::path exe_dir = fs::canonical( argv[0] ).parent_path();

	fs::path nvim_inst_dir = exe_dir / CFG_NVIM_FOLDER_NAME / CFG_INST_FOLDER_NAME;
	fs::path nvim_conf_dir = exe_dir / CFG_NVIM_FOLDER_NAME / CFG_CONF_FOLDER_NAME;
	fs::path nvim_bin_dir = nvim_inst_dir / "bin";

	fs::path alac_inst_dir = exe_dir / CFG_ALAC_FOLDER_NAME / CFG_INST_FOLDER_NAME;
	fs::path alac_conf_dir = exe_dir / CFG_ALAC_FOLDER_NAME / CFG_CONF_FOLDER_NAME;

	if (
		!fs::is_directory( nvim_inst_dir ) ||
		!fs::is_directory( nvim_conf_dir ) ||
		!fs::is_directory( nvim_bin_dir ) ||
		!fs::is_directory( alac_inst_dir ) ||
		!fs::is_directory( alac_conf_dir ) )
	{
		std::cout << "Unable to find the resources required to run the installation\n";
		std::cout << "This could be caused due to having installed the Development Toolkit already" << std::endl;
		return 3;
	}

	// Get user profile and installation directories
	fs::path user_profile_dir = get_user_profile_dir();
	if ( user_profile_dir.empty() ) {
		std::cout << "Unable to get the current user profile directory\n";
		std::cout << "Please try again" << std::endl;
		return 4;
	}

	if (
		!fs::is_directory( user_profile_dir / "AppData" / "Local" ) ||
		!fs::is_directory( user_profile_dir / "AppData" / "Roaming" ) )
	{
		std::cout << "Unable to find application data directories\n";
		std::cout << "What the hell is wrong with your Windows installation?" << std::endl;
		return 5;
	}

	fs::path nvim_target_dir = user_profile_dir / "AppData" / "Local" / "nvim";
	fs::path alac_target_dir = user_profile_dir / "AppData" / "Roaming" / "alacritty";

	for ( const fs::path& target_dir : { nvim_target_dir, alac_target_dir } ) {
		if ( fs::exists( target_dir ) ) {
			fs::path old_dir = target_dir.string() + CFG_OLD_FOLDER_SUFFIX;
			if ( fs::exists( old_dir ) )
				fs::remove_all( old_dir );
			fs::rename( target_dir, old_dir );
		}

		// fs::create_directory( target_dir );
	}

	// Expand bootstrapper macros
	expand_macros_recursive( nvim_conf_dir, user_profile_dir, nvim_inst_dir, nvim_target_dir );
	expand_macros_recursive( alac_conf_dir, user_profile_dir, alac_inst_dir, alac_target_dir );

	// Move config files to target directories
	fs::rename( nvim_conf_dir, nvim_target_dir );
	fs::rename( alac_conf_dir, alac_target_dir );

	// Add Neovim binary directory into PATH
	HKEY user_environment;
	if ( ::RegOpenKeyExA( HKEY_CURRENT_USER, "Environment", 0, KEY_READ | KEY_WRITE, &user_environment ) != ERROR_SUCCESS ) {
		std::cout << "Unable to access user environment variables\n";
		std::cout << "Please try again" << std::endl;
		return 6;
	}

	DWORD env_path_size;
	if ( ::RegGetValueA( user_environment, NULL, "Path", RRF_RT_REG_EXPAND_SZ | RRF_NOEXPAND, NULL, NULL, &env_path_size ) != ERROR_SUCCESS ) {
		std::cout << "Unable to read from user environment variables\n";
		std::cout << "Please try again" << std::endl;
		return 7;
	}

	char env_path[ env_path_size ];
	if ( ::RegGetValueA( user_environment, NULL, "Path", RRF_RT_REG_EXPAND_SZ | RRF_NOEXPAND, NULL, &env_path, &env_path_size ) != ERROR_SUCCESS ) {
		std::cout << "Unable to read from user environment variables\n";
		std::cout << "Please try again" << std::endl;
		return 7;
	}

	std::string env_path_new = std::string( env_path ) + nvim_bin_dir.string() + ';';
	if ( ::RegSetValueExA( user_environment, "Path", 0, REG_EXPAND_SZ, reinterpret_cast<const BYTE*>( env_path_new.c_str() ), env_path_new.size() + 1 ) != ERROR_SUCCESS ) {
		std::cout << "Unable to write to user environment variables\n";
		std::cout << "Please try again" << std::endl;
		return 8;
	}

	if ( ::RegCloseKey( user_environment ) != ERROR_SUCCESS ) {
		std::cout << "Unable to flush user environment variables\n";
		std::cout << "Data might have been corrupted..?\n";
		std::cout << "Please check your system registry for damages\n";
		std::cout << "...or get a computer that doesnt suck" << std::endl;
		return 9;
	}

	// Force all applications to reload environment variables
	DWORD_PTR message_return;
	if ( !::SendMessageTimeoutA( HWND_BROADCAST, WM_SETTINGCHANGE, NULL, LPARAM( "Environment" ), SMTO_BLOCK, 5000, &message_return ) ) {
		std::cout << "Unable to refresh environment variables\n";
		std::cout << "Please modify and refresh them yourself through Control Panel" << std::endl;
		return 10;
	}

	// Finish installation
	std::cout << "Successfully installed Nobody4931's Development Toolkit on this computer!\n";
	std::cout << "If you are 'me', you should already know exactly what to do next" << std::endl;

	return 0;
}
