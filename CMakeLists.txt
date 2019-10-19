# File:           CMakeLists.txt
# Description:     Code Description
# Author:		    Reinaldo Molina
# Email:          rmolin88 at gmail dot com
# Revision:	    0.0.0
# Created:        Tue Mar 05 2019 06:13
# Last Modified:  Tue Mar 05 2019 06:13

# 3.11 - FetchContent
cmake_minimum_required(VERSION 3.11)

# Set ProjectName
project(gnvim)

option(DOWNLOAD_DEPENDENCIES "Download Dependencies" OFF)
if (DOWNLOAD_DEPENDENCIES)
	message (STATUS "Downloading dependencies ...")
	include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/download/fetch_deps.cmake")
else ()
	message (STATUS "Searching for dependencies ...")
	find_package(GTest REQUIRED)

	set(CMAKE_THREAD_PREFER_PTHREAD ON)
	find_package(Threads REQUIRED)

	# set(BUILD_SHARED_LIBS FALSE)
	find_package(SFML 2 COMPONENTS network system graphics REQUIRED)
endif ()


# Download some dependencies
file(GLOB GNVIM_EXTERNAL_SUBDIR
	"${CMAKE_CURRENT_SOURCE_DIR}/cmake/external/*.cmake")

foreach (GNVIM_DEPS ${GNVIM_EXTERNAL_SUBDIR})
	include(${GNVIM_DEPS})
endforeach ()

if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE "Debug" CACHE STRING
		"Choose the type of build, options are: Debug Release
		RelWithDebInfo MinSizeRel."
		FORCE)
endif(NOT CMAKE_BUILD_TYPE)

option (ENABLE_ASAN "Enable ASAN" OFF)
if (ENABLE_ASAN)
	message (STATUS "ASAN enabled")
	set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address")
endif ()

option (ENABLE_USAN "Enable USAN" OFF)
if (ENABLE_USAN)
	message (STATUS "USAN enabled")
	set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=undefined")
endif ()

option(ENABLE_TSAN "Enable TSAN" OFF)
if (ENABLE_TSAN)
	message(STATUS "TSAN enabled")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=thread")
endif ()

option(USE_CLANG "build application with clang" ON) # ON is the default
if (USE_CLANG)
	if (UNIX AND NOT APPLE)
		set (CMAKE_CXX_COMPILER "/usr/bin/clang++" )
		# TODO use the prefix compilar flag
		set (CMAKE_AR           "/usr/bin/llvm-ar" )
		set (CMAKE_LINKER       "/usr/bin/llvm-ld" )
		set (CMAKE_NM           "/usr/bin/llvm-nm" )
		set (CMAKE_OBJDUMP      "/usr/bin/llvm-objdump" )
		set (CMAKE_RANLIB       "/usr/bin/llvm-ranlib" )
	endif (UNIX AND NOT APPLE)
endif (USE_CLANG)

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++17" COMPILER_SUPPORTS_CXX17)

# Build with c++17 support
IF(COMPILER_SUPPORTS_CXX17)
	SET(CMAKE_CXX_STANDARD 17)
ELSE()
	MESSAGE(FATAL_ERROR
		"The compiler ${CMAKE_CXX_COMPILER} has no C++17 support.")
ENDIF()

# add flags to release mode
# by default won't use release mode
if (MSVC)
	# FIXME: put Windows specific options here.
else ()
	set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS}")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fno-strict-aliasing")
	set(CMAKE_CXX_FLAGS_RELEASE
		"${CMAKE_CXX_FLAGS_RELEASE} -Wall -Wfatal-errors -Wextra -Wunused -Werror")
	if (USE_CLANG)
		set(CMAKE_CXX_FLAGS_RELEASE
			"${CMAKE_CXX_FLAGS_RELEASE} -Wno-unused-lambda-capture")
	endif (USE_CLANG)

	set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS}")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g3 -ggdb3 -pedantic")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-strict-aliasing")
	set(CMAKE_CXX_FLAGS_DEBUG
		"${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wextra -Wunused -Werror")
	if (USE_CLANG)
		set(CMAKE_CXX_FLAGS_DEBUG
			"${CMAKE_CXX_FLAGS_DEBUG} -Wno-unused-lambda-capture")
	endif (USE_CLANG)
endif ()

# Specify output directories.
set(EXECUTABLE_OUTPUT_PATH "${PROJECT_BINARY_DIR}/bin")

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

add_subdirectory("src")
# TODO Fix cmake for doxygen
# add_subdirectory("docs")

option(ENABLE_TESTS "Build tests" ON)
if(ENABLE_TESTS)
	enable_testing()
	add_subdirectory("tests")
endif()
