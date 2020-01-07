include(ExternalProject)

find_package(reprocxx)
if (reprocxx_FOUND)
	set(reprocxx_LIBRARIES reprocxx)
	return ()
endif()

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
		PREFIX ${reproc_PREFIX}
		GIT_REPOSITORY https://github.com/DaanDeMeyer/reproc
		GIT_TAG				 v10.0.3
		# GIT_TAG				 18bb97d74db747afbe27f14b122189cbecb39b8d
		GIT_SHALLOW		 TRUE
		GIT_PROGRESS	 TRUE
		CMAKE_ARGS -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
							-DCMAKE_INSTALL_PREFIX=${reproc_INSTALL}
							-DREPROC_TEST=OFF
							-DREPROC_EXAMPLES=OFF
							-DREPROCXX=ON
							# -DREPROC++=ON

		BUILD_BYPRODUCTS ${reproc_LIBS}
		UPDATE_COMMAND ""
		TEST_COMMAND ""
		LOG_DOWNLOAD TRUE
		LOG_INSTALL TRUE
	)

set(reprocxx_FOUND TRUE)
set(reprocxx_INCLUDE_DIRS ${reproc_INSTALL}/include)
set(reprocxx_LIBRARIES ${reproc_LIBS})
set(reprocxx_LIBRARY_DIRS ${reproc_INSTALL}/lib)
set(reprocxx_EXTERNAL TRUE)
