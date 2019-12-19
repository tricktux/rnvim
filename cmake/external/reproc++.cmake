include(ExternalProject)

# build directory
set(reproc_PREFIX ${CMAKE_BINARY_DIR}/external/reproc-prefix)
# install directory
set(reproc_INSTALL ${CMAKE_BINARY_DIR}/external/reproc-install)

set(reproc_LIBS
	${reproc_INSTALL}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}reproc++${CMAKE_STATIC_LIBRARY_SUFFIX}
	${reproc_INSTALL}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}reproc${CMAKE_STATIC_LIBRARY_SUFFIX}
	)

ExternalProject_Add(
	reproc
	PREFIX ${libnvc_PREFIX}
	GIT_REPOSITORY https://github.com/DaanDeMeyer/reproc
	# Using latest to match example code
	# GIT_TAG				 v9.0.0
	GIT_SHALLOW		 TRUE
	GIT_PROGRESS	 TRUE
	CMAKE_ARGS -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
						-DCMAKE_INSTALL_PREFIX=${reproc_INSTALL}
						-DREPROC++=ON
						-DREPROC_TEST=OFF
						-DREPROC_EXAMPLES=OFF

	UPDATE_DISCONNECTED TRUE
	BUILD_BYPRODUCTS ${reproc_LIBS}
	LOG_DOWNLOAD TRUE
	LOG_INSTALL TRUE
	)

set(REPROC_FOUND TRUE)
set(REPROC_INCLUDE_DIRS ${reproc_INSTALL}/include)
set(REPROC_LIBRARIES ${reproc_LIBS})
set(REPROC_LIBRARY_DIRS ${reproc_INSTALL}/lib)
set(REPROC_EXTERNAL TRUE)
