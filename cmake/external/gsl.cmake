include(ExternalProject)

# build directory
set(gsl_PREFIX ${CMAKE_BINARY_DIR}/external/gsl-prefix)
# install directory
set(gsl_INSTALL ${CMAKE_BINARY_DIR}/external/gsl-install)

ExternalProject_Add(
	gsl
	PREFIX ${gsl_PREFIX}
	GIT_REPOSITORY https://github.com/microsoft/GSL
	GIT_TAG				 v2.0.0
	GIT_SHALLOW		 1
	GIT_PROGRESS	 1
	CMAKE_ARGS -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
						-DCMAKE_INSTALL_PREFIX=${gsl_INSTALL}
						-DGSL_TEST=OFF
						-DGSL_CXX_STANDARD=${CXX_COMPILER_FLAG}
	)

set(GSL_FOUND TRUE)
set(GSL_INCLUDE_DIRS ${gsl_INSTALL}/include)
set(GSL_EXTERNAL TRUE)
