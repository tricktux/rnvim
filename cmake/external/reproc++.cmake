include(ExternalProject)

# build directory
set(reproc++_PREFIX ${CMAKE_BINARY_DIR}/external/reproc++-prefix)
# install directory
set(reproc++_INSTALL ${CMAKE_BINARY_DIR}/external/reproc++-install)

ExternalProject_Add(
	reproc++
	PREFIX ${libnvc_PREFIX}
	GIT_REPOSITORY https://github.com/DaanDeMeyer/reproc
	GIT_TAG				 v9.0.0
	GIT_SHALLOW		 1
	GIT_PROGRESS	 1
	CMAKE_ARGS -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
						-DCMAKE_INSTALL_PREFIX=${reproc++_INSTALL}
						-DREPROC++=ON
						-DREPROC_TEST=OFF
						-DREPROC_EXAMPLES=OFF

	LOG_DOWNLOAD 1
	LOG_INSTALL 1
	)

set(REPROC++_FOUND TRUE)
set(REPROC++_INCLUDE_DIRS ${reproc++_INSTALL}/include)
set(REPROC++_EXTERNAL TRUE)
