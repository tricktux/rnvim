include(ExternalProject)

# build directory
set(nvc_PREFIX ${CMAKE_BINARY_DIR}/external/nvc-prefix)
# install directory
set(nvc_INSTALL ${CMAKE_BINARY_DIR}/external/nvc-install)

find_package (Threads)

set(nvc_LIBS
	${nvc_INSTALL}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}nvc${CMAKE_STATIC_LIBRARY_SUFFIX}
	)

ExternalProject_Add(
	nvc
	PREFIX ${nvc_PREFIX}
	GIT_REPOSITORY https://github.com/etorth/libnvc
	GIT_SHALLOW		 TRUE
	GIT_PROGRESS	 TRUE
	CMAKE_ARGS -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
					-DCMAKE_INSTALL_PREFIX=${nvc_INSTALL}

	BUILD_BYPRODUCTS ${nvc_LIBS}
	LOG_DOWNLOAD TRUE
	LOG_INSTALL TRUE
	)

set(NVC_FOUND TRUE)
set(NVC_INCLUDE_DIRS ${nvc_INSTALL}/include)
set(NVC_LIBRARIES ${nvc_LIBS} ${CMAKE_THREAD_LIBS_INIT})
set(NVC_LIBRARY_DIRS ${nvc_INSTALL}/lib)
set(NVC_EXTERNAL TRUE)
