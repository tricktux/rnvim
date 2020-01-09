# File:           CMakeLists.txt
# Description:     Code Description
# Author:		    Reinaldo Molina
# Email:          rmolin88 at gmail dot com
# Revision:	    0.0.0
# Created:        Tue Mar 05 2019 06:13
# Last Modified:  Tue Mar 05 2019 06:13

# 3.13 - updated target_sources policy
cmake_minimum_required(VERSION 3.13)

# Set ProjectName
project(rnvim)

# Manually include dependencies
set (_modules
		"${CMAKE_SOURCE_DIR}/cmake"
		"${CMAKE_SOURCE_DIR}/cmake/sanitizers"
	)

set (CMAKE_MODULE_PATH _modules)

# include("${CMAKE_SOURCE_DIR}/cmake/external/gsl.cmake")
# include("${CMAKE_SOURCE_DIR}/cmake/external/fmt.cmake")
# include("${CMAKE_SOURCE_DIR}/cmake/external/mpack.cmake")
# include("${CMAKE_SOURCE_DIR}/cmake/external/libnvc.cmake")
include("${CMAKE_SOURCE_DIR}/cmake/external/easylogging.cmake")
# include("${CMAKE_SOURCE_DIR}/cmake/external/cxxopts.cmake")
include("${CMAKE_SOURCE_DIR}/cmake/external/reproc++.cmake")
include("${CMAKE_SOURCE_DIR}/cmake/jinja2.cmake")
include("${CMAKE_SOURCE_DIR}/third-party/mpack/mpack.cmake")

# Set a default build type if none was specified
set(CMAKE_BUILD_TYPE "Debug" CACHE
	STRING "Choose the type of build." FORCE)
# Set the possible values of build type for cmake-gui
set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
	"Debug" "Release" "MinSizeRel" "RelWithDebInfo")

SET(CMAKE_COLOR_MAKEFILE ON)

# Options
# You can achieve the same behavior by doing CC=clang CXX=clang++ cmake ..
option(USE_CLANG "Build application with clang" OFF) # Ignored at the moment
# Depends on clang and llvm being installed
# cmake -DSANITIZE_ADDRESS=ON ..
# ./bin/<executable_name>
option(SANITIZE_ADDRESS "Enable Address Sanitizer" OFF)
option(SANITIZE_MEMORY "Enable Memory Sanitizer" OFF)
option(SANITIZE_THREAD "Enable Thread Sanitizer" OFF)
option(SANITIZE_UNDEFINED "Enable Undefined Behavior Sanitizer" OFF)
option(SANITIZE_LINK_STATIC "Try to link static against sanitizers." OFF)

option(ENABLE_TESTS "Build tests" ON)
option(ENABLE_LIBNVC_TESTS "Build tests" OFF)
option(ENABLE_CLANG_TIDY "Use clang-tidy" OFF)

if (ENABLE_CLANG_TIDY)
	find_program (CLANG_TIDY_EXE NAMES "clang-tidy")
	if(CLANG_TIDY_EXE)
		set(CMAKE_CXX_CLANG_TIDY
			"${CLANG_TIDY_EXE};-format-style=file;-checks=*" CACHE STRING "" FORCE)
	else()
		message(FATAL_ERROR "clang-tidy NOT found!")
		set(CMAKE_CXX_CLANG_TIDY "" CACHE STRING "" FORCE) # clear it
	endif()
endif()

# Build with c++17 support
include(CheckCXXCompilerFlag)
set (_cpp_version "17")
if (MSVC AND MSVC_VERSION GREATER_EQUAL "1900")
	check_cxx_compiler_flag("/std:c++latest" _cpp_latest_flag_supported)
	if (_cpp_latest_flag_supported)
		add_compile_options("/std:c++latest")
	endif()
else ()
	check_cxx_compiler_flag("-std=c++${_cpp_version}"
		_cpp_latest_flag_supported)
	if (_cpp_latest_flag_supported)
		add_compile_options("-std=c++${_cpp_version}")
	endif()
endif()

# add flags to release mode
# by default won't use release mode
if (MSVC)
	add_compile_options(/W4 /WX /EHsc)
else ()
	set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS}")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fno-strict-aliasing")
	set(CMAKE_CXX_FLAGS_RELEASE
		"${CMAKE_CXX_FLAGS_RELEASE} -Wall -Wfatal-errors -Wextra -Wunused -Werror")
	if (USE_CLANG OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
		set(CMAKE_CXX_FLAGS_RELEASE
			"${CMAKE_CXX_FLAGS_RELEASE} -Wno-unused-lambda-capture")
	endif ()

	set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS}")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")
	# set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fdiagnostics-color=always")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g3 -ggdb3 -pedantic")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-strict-aliasing")
	set(CMAKE_CXX_FLAGS_DEBUG
		"${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wextra -Wunused -Werror")
	if (USE_CLANG OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
		set(CMAKE_CXX_FLAGS_DEBUG
			"${CMAKE_CXX_FLAGS_DEBUG} -Wno-unused-lambda-capture")
	endif ()
endif ()

# Specify output directories.
# Place targets in bin/ or lib/ for all build configurations
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
foreach(CFGNAME ${CMAKE_CONFIGURATION_TYPES})
	string(TOUPPER ${CFGNAME} CFGNAME)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${CFGNAME} ${CMAKE_BINARY_DIR}/bin)
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${CFGNAME} ${CMAKE_BINARY_DIR}/lib)
	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${CFGNAME} ${CMAKE_BINARY_DIR}/lib)
endforeach()
message( STATUS "RUNTIME_OUTPUT_DIR: " ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} )

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Find packages
find_package(Threads REQUIRED)
find_package(fmt)

if (SANITIZE_ADDRESS OR SANITIZE_MEMORY OR SANITIZE_THREAD OR
		SANITIZE_UNDEFINED)
	find_package(Sanitizers REQUIRED)
endif ()


# add_subdirectory("lib")
add_subdirectory("src")
# TODO Fix cmake for doxygen
# add_subdirectory("docs")

if(ENABLE_TESTS AND EXISTS "${CMAKE_SOURCE_DIR}/tests")
	message(STATUS "Building Tests")
	find_package(GTest REQUIRED)
	enable_testing()
	add_subdirectory("tests")
endif()

# Copy configuration files
configure_file(
		"${CMAKE_SOURCE_DIR}/config/easylogingpp.conf"
		"${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/easylogingpp.conf"
		COPYONLY
	)

include (GNUInstallDirs)
# install (TARGETS aws-text)
# Copy all config files to /etc
if (EXISTS "${CMAKE_SOURCE_DIR}/config")
	install (FILES
		GLOB "${CMAKE_SOURCE_DIR}/config/*.*"
		DESTINATION ${CMAKE_INSTALL_SYSCONFDIR}
		)
endif ()
