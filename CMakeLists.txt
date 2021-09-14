cmake_minimum_required(VERSION 3.20 FATAL_ERROR)

set(CMAKE_BUILD_TYPE Release)
project(4931_bootstrapper
	VERSION 1.0.0
	DESCRIPTION "Quickly unpacks and sets up my development toolkit on any Windows computer"
	LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(PROJECT_LIB_FOLDER "${CMAKE_CURRENT_SOURCE_DIR}/lib")
# set(PROJECT_DEP_FOLDER "${CMAKE_CURRENT_SOURCE_DIR}/dep")

add_compile_options("-O3")
add_compile_options("-stdlib=libstdc++")

add_subdirectory(src)
