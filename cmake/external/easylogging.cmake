include(ExternalProject)

# build directory
set(easylogging_PREFIX ${CMAKE_BINARY_DIR}/external/easylogging-prefix)
# install directory
set(easylogging_INSTALL ${CMAKE_BINARY_DIR}/external/easylogging-install)

ExternalProject_Add(
	easylogging
	PREFIX ${easylogging_PREFIX}
	GIT_REPOSITORY https://github.com/zuhd-org/easyloggingpp
	GIT_TAG				 v9.96.7
	GIT_SHALLOW		 1
	GIT_PROGRESS	 1
	CMAKE_ARGS -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
						-DCMAKE_INSTALL_PREFIX=${easylogging_INSTALL}
						-Dbuild_static_lib=ON

	LOG_DOWNLOAD 1
	LOG_INSTALL 1
)

set(EASYLOGGING_FOUND TRUE)
set(EASYLOGGING_INCLUDE_DIRS ${easylogging_INSTALL}/include)
set(EASYLOGGING_LIBRARIES ${easylogging_INSTALL}/lib/libeasyloggingpp.a)
set(EASYLOGGING_LIBRARY_DIRS ${easylogging_INSTALL}/lib)
set(EASYLOGGING_EXTERNAL TRUE)
