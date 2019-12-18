include(ExternalProject)

# build directory
set(fmt_PREFIX ${CMAKE_BINARY_DIR}/external/fmt-prefix)
# install directory
set(fmt_INSTALL ${CMAKE_BINARY_DIR}/external/fmt-install)

set(fmt_LIBS
		${fmt_INSTALL}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}fmt${CMAKE_STATIC_LIBRARY_SUFFIX}
	)

ExternalProject_Add(
		fmt
		PREFIX ${fmt_PREFIX}
		GIT_REPOSITORY https://github.com/fmtlib/fmt
		GIT_TAG				 6.1.2
		GIT_SHALLOW		 TRUE
		GIT_PROGRESS	 TRUE
		CMAKE_ARGS -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
		-DCMAKE_INSTALL_PREFIX=${fmt_INSTALL}
		-DBUILD_SHARED_LIBS=OFF
		-DFMT_DOC=OFF
		-DFMT_TEST=OFF
		-DFMT_FUZZ=OFF
		-DFMT_CUDA_TEST=OFF

		BUILD_BYPRODUCTS ${fmt_LIBS}
		LOG_DOWNLOAD TRUE
		LOG_INSTALL TRUE
	)

set(FMT_FOUND TRUE)
set(FMT_INCLUDE_DIRS ${fmt_INSTALL}/include)
set(FMT_LIBRARIES ${fmt_LIBS})
set(FMT_LIBRARY_DIRS ${fmt_INSTALL}/lib)
set(FMT_EXTERNAL TRUE)
