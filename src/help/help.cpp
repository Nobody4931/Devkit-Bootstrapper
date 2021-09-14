#include "help/help.hpp"

#include <iostream>


int program_help( int argc, char** argv ) {
	std::cout << '\n';

	std::cout << "4931 Development Toolkit Bootstrapper v1.0.0a\n";
	std::cout << "Created by Nobody4931 for personal use\n\n";

	std::cout << "For quickly unpacking and setting up my development toolkit on any Windows computer\n\n";

	std::cout << "Usage:\n";
	std::cout << "\t4931_bootstrapper.exe <operation>\n\n";

	std::cout << "Available operations:\n";
	std::cout << "\thelp      | Shows this dialogue\n";
	std::cout << "\tinstall   | Unpacks and installs the development toolkit\n";
	std::cout << "\tuninstall | Uninstalls and removes all installed items\n";
	std::cout << "\tadjust    | Automatically adjusts the size of the terminal\n";

	std::cout << std::endl;
	return 0;
}
