
include(FetchContent)

FetchContent_Declare(
		glog
		GIT_REPOSITORY https://github.com/google/glog
		GIT_TAG        v0.4.0
	)
FetchContent_Declare(
		reproc
		GIT_REPOSITORY https://github.com/DaanDeMeyer/reproc
		GIT_TAG        v4.0.0
	)

# After the following call, the CMake targets defined by googletest
# will be defined and available to the rest of the build
FetchContent_MakeAvailable(glog)

# If you need to add special steps or variables do this
FetchContent_GetProperties(reproc)
if(NOT reproc_POPULATED)
	FetchContent_Populate(reproc)

	# Build the C++ version of reproc as well
	set(REPROC++ ON CACHE BOOL "" FORCE)
	add_subdirectory(${reproc_SOURCE_DIR} ${reproc_BINARY_DIR})
endif()
