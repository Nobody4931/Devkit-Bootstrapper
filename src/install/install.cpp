#include "install/install.hpp"

#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;


int program_install( int argc, char** argv ) {
	fs::path exe_dir = fs::canonical( argv[0] ).parent_path();

	fs::path nvim_inst_dir = exe_dir / "neovim" / "inst";
	fs::path nvim_conf_dir = exe_dir / "neovim" / "conf";

	fs::path alac_inst_dir = exe_dir / "alacritty" / "inst";
	fs::path alac_conf_dir = exe_dir / "alacritty" / "conf";

	if (
		!fs::exists( nvim_inst_dir ) ||
		!fs::exists( nvim_conf_dir ) ||
		!fs::exists( alac_inst_dir ) ||
		!fs::exists( alac_conf_dir ) )
	{
		std::cout << "Unable to find the resources required to run the installation\n";
		std::cout << "If you are 'me', then that probably means I failed to set up this flash drive properly lol" << std::endl;
		return 3;
	}

	return 0;
}
