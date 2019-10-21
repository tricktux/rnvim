include(ExternalProject)

# build directory
set(libnvc_PREFIX ${CMAKE_BINARY_DIR}/external/libnvc-prefix)
# install directory
set(libnvc_INSTALL ${CMAKE_BINARY_DIR}/external/libnvc-install)

find_package (Threads)
ExternalProject_Add(
	libnvc
	PREFIX ${libnvc_PREFIX}
	GIT_REPOSITORY https://github.com/etorth/libnvc
	GIT_SHALLOW		 1
	GIT_PROGRESS	 1
	CMAKE_ARGS -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
					-DCMAKE_INSTALL_PREFIX=${libnvc_INSTALL}
					-DENABLE_ASAN=${ENABLE_ASAN}
					-DENABLE_USAN=${ENABLE_USAN}
					-DENABLE_TSAN=${ENABLE_TSAN}

	LOG_DOWNLOAD 1
	LOG_INSTALL 1
	)

set(LIBNVC_FOUND TRUE)
set(LIBNVC_INCLUDE_DIRS ${libnvc_INSTALL}/include)
set(LIBNVC_LIBRARIES ${libnvc_INSTALL}/lib/libnvc.a ${CMAKE_THREAD_LIBS_INIT})
set(LIBNVC_LIBRARY_DIRS ${libnvc_INSTALL}/lib)
set(LIBNVC_EXTERNAL TRUE)
