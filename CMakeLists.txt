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
	include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/download/fetch.cmake")
else ()
	find_package(gflags REQUIRED)

	find_package(GTest REQUIRED)

	set(CMAKE_THREAD_PREFER_PTHREAD ON)
	find_package(Threads REQUIRED)

	# set(BUILD_SHARED_LIBS FALSE)
	find_package(SFML 2 COMPONENTS network system graphics REQUIRED)
endif ()


# Download some dependencies
file(GLOB GNVIM_DEPS_SUBDIR
	"${CMAKE_CURRENT_SOURCE_DIR}/cmake/deps/*.cmake")
foreach (GNVIM_DEPS ${GNVIM_DEPS_SUBDIR})
	include(${GNVIM_DEPS})
endforeach ()

# Check libnvc
if (DEFINED LIBNVC_INCLUDE)
	message (STATUS "Found libnvc include path: ${LIBNVC_INCLUDE}")
else ()
	message (FATAL_ERROR
		"Required LIBNVC_INCLUDE=\"to-libnvc-include-path\" not defined")
endif ()

if (DEFINED LIBNVC_LIB)
	message (STATUS "Found libnvc library path: ${LIBNVC_LIB}")
else ()
	message (FATAL_ERROR
		"Required LIBNVC_LIB=\"to-libnvc-library-path\" not defined")
endif ()

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
	set (CMAKE_CXX_COMPILER "/usr/bin/clang++" )
	set (CMAKE_AR           "/usr/bin/llvm-ar" )
	set (CMAKE_LINKER       "/usr/bin/llvm-ld" )
	set (CMAKE_NM           "/usr/bin/llvm-nm" )
	set (CMAKE_OBJDUMP      "/usr/bin/llvm-objdump" )
	set (CMAKE_RANLIB       "/usr/bin/llvm-ranlib" )
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
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS}")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fno-strict-aliasing")
set(CMAKE_CXX_FLAGS_RELEASE
	"${CMAKE_CXX_FLAGS_RELEASE} -Wall -Wfatal-errors -Wextra -Wunused -Werror")
set(CMAKE_CXX_FLAGS_RELEASE
	"${CMAKE_CXX_FLAGS_RELEASE} -Wno-unused-lambda-capture")

# always compile libnvc in debug mode
# for trace message use LIBNVC_DEBUG_ARG_LIST to enable
set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS}")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g3 -ggdb3 -pedantic")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-strict-aliasing")
set(CMAKE_CXX_FLAGS_DEBUG
	"${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wextra -Wunused -Werror")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wno-unused-lambda-capture")

# Specify output directories.
set(EXECUTABLE_OUTPUT_PATH "${PROJECT_BINARY_DIR}/bin")

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

add_subdirectory("src")
# TODO Fix cmake for doxygen
# add_subdirectory("docs")

option(ENABLE_TESTS "Build tests" OFF)
if(ENABLE_TESTS)
	enable_testing()
	add_subdirectory("tests")
endif()
