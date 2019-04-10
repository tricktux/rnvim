
include(FetchContent)

# set(EXTERNAL_INSTALL_LOCATION ${CMAKE_BINARY_DIR}/external)

FetchContent_Declare(
	googletest
	GIT_REPOSITORY https://github.com/google/googletest.git
	GIT_TAG        release-1.8.1
	)
FetchContent_Declare(
	glog
	GIT_REPOSITORY https://github.com/google/glog/releases
	GIT_TAG        v0.4.0
	)
FetchContent_Declare(
	gflags
	GIT_REPOSITORY https://github.com/gflags/gflags/releases
	GIT_TAG        v2.2.2
	)
FetchContent_Declare(
	reproc
	GIT_REPOSITORY https://github.com/DaanDeMeyer/reproc
	GIT_TAG        v4.0.0
	)

# After the following call, the CMake targets defined by googletest and
# Catch2 will be defined and available to the rest of the build
FetchContent_MakeAvailable(googletest glog gflags)

# ExternalProject_Add(libnvc
	# GIT_REPOSITORY https://github.com/etorth/libnvc
	# CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${EXTERNAL_INSTALL_LOCATION}
		# -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
	# )

# SET(LIBNVC_INCLUDE "${EXTERNAL_INSTALL_LOCATION}/inc")
# SET(LIBNVC_LIB "${EXTERNAL_INSTALL_LOCATION}/lib")

# include_directories(${EXTERNAL_INSTALL_LOCATION}/include)
# link_directories(${EXTERNAL_INSTALL_LOCATION}/lib)
