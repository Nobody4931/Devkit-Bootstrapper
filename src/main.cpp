#include <iostream>
#include <string>
#include <map>

#include "help/help.hpp"
#include "install/install.hpp"
#include "uninstall/uninstall.hpp"

#if _WIN32 == 1
#	include "adjust/adjust.hpp"
#endif // _WIN32 == 1


static std::map<std::string, int(*)( int, char** )> program_operations = {
	{ "help",      &program_help },
	{ "install",   &program_install },
	{ "uninstall", &program_uninstall },
#if _WIN32 == 1
	{ "adjust",    &program_adjust },
#endif // _WIN32 == 1
};


int main( int argc, char** argv ) {
	if ( argc != 2 ) {
		std::cout << "Invalid amount of arguments provided\n";
		std::cout << "Expected 1, got " << ( argc - 1 ) << std::endl;
		return 1;
	}

	auto operation_it = program_operations.find( argv[1] );
	if ( operation_it == program_operations.end() ) {
		std::cout << '\'' << argv[1] << "' is not a valid operation\n";
		std::cout << "Run this application again with 'help' as an argument for instructions" << std::endl;
		return 2;
	}

	return operation_it->second( argc, argv );
}
