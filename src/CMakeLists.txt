list(APPEND PROJECT_LIBS
	"help/help.cpp")

list(APPEND PROJECT_LIBS
	"install/install.cpp")

list(APPEND PROJECT_LIBS
	"uninstall/uninstall.cpp")

list(APPEND PROJECT_LIBS
	"adjust/adjust.cpp")

add_executable(${PROJECT_NAME}
	${PROJECT_LIBS}
	main.cpp)

target_include_directories(${PROJECT_NAME} PRIVATE
	"${PROJECT_LIB_FOLDER}")
