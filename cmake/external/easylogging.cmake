include(ExternalProject)

# build directory
set(easylogging_PREFIX ${CMAKE_BINARY_DIR}/external/easylogging-prefix)
# install directory
set(easylogging_INSTALL ${CMAKE_BINARY_DIR}/external/easylogging-install)

set(easylogging_LIBS
  ${easylogging_INSTALL}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}easyloggingpp${CMAKE_STATIC_LIBRARY_SUFFIX}
	)

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
set(EASYLOGGING_SOURCES
      ${EASYLOGGING_INCLUDE_DIRS}/easylogging++.h
      ${EASYLOGGING_INCLUDE_DIRS}/easylogging++.cc
)
set(EASYLOGGING_LIBRARIES ${easylogging_LIBS})
set(EASYLOGGING_LIBRARY_DIRS ${easylogging_INSTALL}/lib)
set(EASYLOGGING_EXTERNAL TRUE)

# Needed due to:
#   https://gitlab.kitware.com/cmake/cmake/issues/15052
file(MAKE_DIRECTORY ${EASYLOGGING_INCLUDE_DIRS})

add_library(easyloggingpp STATIC IMPORTED GLOBAL)
add_dependencies(easyloggingpp easylogging)
add_library(easyloggingpp::easyloggingpp ALIAS easyloggingpp)
set_target_properties(easyloggingpp PROPERTIES
    IMPORTED_LOCATION ${EASYLOGGING_LIBRARIES}
    INTERFACE_INCLUDE_DIRECTORIES ${EASYLOGGING_INCLUDE_DIRS}
    INTERFACE_COMPILE_DEFINITIONS
      "ELPP_STL_LOGGING;ELPP_LOG_STD_ARRAY;ELPP_LOG_UNORDERED_MAP;ELPP_LOG_UNORDERED_SET"
    PUBLIC_HEADER "${EASYLOGGING_INCLUDE_DIRS}/easylogging++.h"
    IMPORTED_LINK_INTERFACE_LIBRARIES "${EASYLOGGING_LIBRARIES}"
    INTERFACE_LINK_DIRECTORIES ${EASYLOGGING_LIBRARY_DIRS}
    INTERFACE_LINK_LIBRARIES ${EASYLOGGING_LIBRARIES}
  )
