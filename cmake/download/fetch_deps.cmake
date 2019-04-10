
include(FetchContent)

FetchContent_Declare(
		googletest
		GIT_REPOSITORY https://github.com/google/googletest.git
		GIT_TAG        release-1.8.1
	)
FetchContent_Declare(
		gflags
		GIT_REPOSITORY https://github.com/gflags/gflags
		GIT_TAG        v2.2.2
	)
FetchContent_Declare(
		sfml
		GIT_REPOSITORY https://github.com/SFML/SFML
		GIT_TAG        2.5.1
	)

# After the following call, the CMake targets defined by googletest
# will be defined and available to the rest of the build
FetchContent_MakeAvailable(googletest glog gflags sfml)

# If you need to add special steps or variables do this
# FetchContent_GetProperties(reproc)
# if(NOT reproc_POPULATED)
	# FetchContent_Populate(reproc)

	# # Build the C++ version of reproc as well
	# set(REPROC++ ON CACHE BOOL "" FORCE)
	# add_subdirectory(${reproc_SOURCE_DIR} ${reproc_BINARY_DIR})
# endif()
