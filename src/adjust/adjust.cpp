#define WIN32_LEAN_AND_MEAN

#include "adjust/adjust.hpp"

#include <windows.h>
#include <iostream>


#define ADJUST_GAP_WIDTH  10
#define ADJUST_GAP_HEIGHT 10


int program_adjust( int argc, char** argv ) {
	// Get Alacritty window
	HWND window_hndl = ::FindWindowA( NULL, "Alacritty" );
	if ( !::IsWindow( window_hndl ) ) {
		std::cout << "Unable to find the terminal window\n";
		std::cout << "Please run the program again after opening Alacritty" << std::endl;
		return 1; // TODO: Fix error code
	}

	// Get desktop work area size
	RECT desktop_size;
	if ( !::SystemParametersInfoA( SPI_GETWORKAREA, 0, &desktop_size, NULL ) ) {
		std::cout << "Unable to get screen resolution\n";
		std::cout << "Please try again" << std::endl;
		return 1; // TODO: Fix error code
	}

	// horizontal: desktop_size.right
	// vertical: desktop_size.bottom

	// std::cout << desktop_size.right << ", " << desktop_size.bottom << '\n';

	// Resize and reposition window
	if ( !::SetWindowPos(
			window_hndl,
			HWND_TOP,
			ADJUST_GAP_WIDTH,
			ADJUST_GAP_HEIGHT,
			desktop_size.right - ADJUST_GAP_WIDTH * 2,
			desktop_size.bottom - ADJUST_GAP_HEIGHT * 2,
			// screen_width - ADJUST_GAP_WIDTH * 2,
			// screen_height - ADJUST_GAP_HEIGHT * 2,
			SWP_SHOWWINDOW ) )
	{
		std::cout << "Unable to adjust window position/size\n";
		std::cout << "Please try again" << std::endl;
		return 1; // TODO: Fix error code
	}


	std::cout << "Successfully adjusted terminal window\n";
	std::cout << "Gap width: " << ADJUST_GAP_WIDTH << '\n';
	std::cout << "Gap height: " << ADJUST_GAP_HEIGHT << '\n';
	std::cout.flush();

	return 0;
}
