include(FetchContent)

# FetchContent_Declare(
		# easylogging
		# GIT_REPOSITORY https://github.com/zuhd-org/easyloggingpp
		# GIT_TAG        v9.96.7
	# )
FetchContent_Declare(
		cxxopts
		GIT_REPOSITORY https://github.com/jarro2783/cxxopts
		GIT_TAG        v2.1.2
	)
FetchContent_Declare(
		reproc
		GIT_REPOSITORY https://github.com/DaanDeMeyer/reproc
		GIT_TAG        v5.0.0
	)

FetchContent_MakeAvailable(cxxopts)

# If you need to add special steps or variables do this
FetchContent_GetProperties(reproc)
if(NOT reproc_POPULATED)
	FetchContent_Populate(reproc)

	# Build the C++ version of reproc as well
	set(REPROC++ ON CACHE BOOL "" FORCE)
	add_subdirectory(${reproc_SOURCE_DIR} ${reproc_BINARY_DIR})
endif()

# FetchContent_GetProperties(easylogging)
# if(NOT easylogging_POPULATED)
	# FetchContent_Populate(easylogging)

	# # Build the C++ version of reproc as well
	# set(build_static_lib ON CACHE BOOL "" FORCE)
	# set(CMAKE_INSTALL_PREFIX ${easylogging_BINARY_DIR})
	# add_subdirectory(${easylogging_SOURCE_DIR} ${easylogging_BINARY_DIR})
# endif()
