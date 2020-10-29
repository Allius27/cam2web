
cmake_minimum_required(VERSION 3.5)

set(DEFAULT_BUILD_TYPE "Release")
set(CMAKE_CXX_STANDARD 14)

project(cam2webCore
    LANGUAGES CXX
)

include_directories ( ../../externals/mongoose )

add_definitions(-DMG_ENABLE_THREADS)

add_library( ${PROJECT_NAME} SHARED 
	../../externals/mongoose/mongoose.cpp
	XError.cpp 
	XManualResetEvent.cpp
	XObjectConfigurationRequestHandler.cpp
	XSimpleJsonParser.cpp
	XStringTools.cpp
	XWebServer.cpp
)

set_target_properties ( ${PROJECT_NAME}
	PROPERTIES 
		PUBLIC_HEADER 
			"XObjectConfigurationRequestHandler.hpp;XWebServer.hpp;XInterfaces.hpp"
)

INSTALL(
	TARGETS ${PROJECT_NAME}

	LIBRARY DESTINATION lib
	PUBLIC_HEADER DESTINATION include/${PROJECT_NAME}
)
