include(ExternalProject)

# build directory
set(easylogging_PREFIX ${CMAKE_BINARY_DIR}/external/easylogging-prefix)
# install directory
set(easylogging_INSTALL ${CMAKE_BINARY_DIR}/external/easylogging-install)

set(easylogging_LIBS
		${easylogging_INSTALL}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}easyloggingpp${CMAKE_STATIC_LIBRARY_SUFFIX}
	)

add_definitions(-DELPP_STL_LOGGING)
add_definitions(-DELPP_LOG_STD_ARRAY)
add_definitions(-DELPP_LOG_UNORDERED_MAP)
add_definitions(-DELPP_LOG_UNORDERED_SET)
add_definitions(-DELPP_FEATURE_CRASH_LOG)
add_definitions(-DELPP_HANDLE_SIGABRT)
add_definitions(-DELPP_EXPERIMENTAL_ASYNC)

ExternalProject_Add(
	easylogging
	PREFIX ${easylogging_PREFIX}
	GIT_REPOSITORY https://github.com/zuhd-org/easyloggingpp
	GIT_TAG				 v9.96.7
	GIT_SHALLOW		 TRUE
	GIT_PROGRESS	 TRUE
	CMAKE_ARGS -DCMAKE_BUILD_TYPE=Release
						-DCMAKE_INSTALL_PREFIX=${easylogging_INSTALL}
						-Dbuild_static_lib=ON

	UPDATE_DISCONNECTED TRUE
	BUILD_BYPRODUCTS ${easylogging_LIBS}
	LOG_DOWNLOAD TRUE
	LOG_INSTALL TRUE
)

set(EASYLOGGING_FOUND TRUE)
set(EASYLOGGING_INCLUDE_DIRS ${easylogging_INSTALL}/include)
set(EASYLOGGING_LIBRARIES ${easylogging_LIBS})
set(EASYLOGGING_LIBRARY_DIRS ${easylogging_INSTALL}/lib)
set(EASYLOGGING_EXTERNAL TRUE)
# TODO if (WIN32) copy dynamic lib to RUNTIME_OUTPUT_DIR?
