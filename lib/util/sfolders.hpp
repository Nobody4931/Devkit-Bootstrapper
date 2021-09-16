#pragma once


#define WIN32_LEAN_AND_MEAN

#include <shlobj_core.h>
#include <filesystem>


inline std::filesystem::path get_user_profile_dir() {
	CHAR user_dir[ MAX_PATH ] { 0 };

	if ( !::SHGetSpecialFolderPathA( NULL, user_dir, CSIDL_PROFILE, false ) )
		return std::filesystem::path();

	return std::filesystem::path( user_dir );
}
