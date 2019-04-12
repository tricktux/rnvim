include(FetchContent)

FetchContent_Declare(
		reproc
		GIT_REPOSITORY https://github.com/DaanDeMeyer/reproc
		GIT_TAG        v4.0.0
	)

# If you need to add special steps or variables do this
FetchContent_GetProperties(reproc)
if(NOT reproc_POPULATED)
	FetchContent_Populate(reproc)

	# Build the C++ version of reproc as well
	set(REPROC++ ON CACHE BOOL "" FORCE)
	add_subdirectory(${reproc_SOURCE_DIR} ${reproc_BINARY_DIR})
endif()
