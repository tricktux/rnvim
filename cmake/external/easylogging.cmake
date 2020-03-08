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
add_library(easyloggingpp::easyloggingpp INTERFACE IMPORTED)
set (easylogging_defines
        ELPP_STL_LOGGING
        ELPP_LOG_STD_ARRAY
        ELPP_LOG_UNORDERED_MAP
        ELPP_LOG_UNORDERED_SET
        )
# set_target_properties(easyloggingpp::easyloggingpp PROPERTIES
    # IMPORTED_LOCATION ${EASYLOGGING_LIBRARIES}
  # )
set_target_properties(easyloggingpp::easyloggingpp PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES
      ${EASYLOGGING_INCLUDE_DIRS}
  )
set_target_properties(easyloggingpp::easyloggingpp PROPERTIES
    INTERFACE_COMPILE_DEFINITIONS ELPP_STL_LOGGING
    )
set_target_properties(easyloggingpp::easyloggingpp PROPERTIES
    INTERFACE_LINK_DIRECTORIES ${EASYLOGGING_LIBRARY_DIRS}
  )
set_target_properties(easyloggingpp::easyloggingpp PROPERTIES
    INTERFACE_LINK_LIBRARIES ${EASYLOGGING_LIBRARIES}
  )

function(target_include_easyloggingpp target)
  # Everywhere you want to include ${JINJA2_SRC}
  # You must copy and paste this line :(.... ask CMake
  # set_source_files_properties(${EASYLOGGING_SOURCES}
    # PROPERTIES GENERATED 1
    # )
	target_compile_definitions(${target}
		PRIVATE
			ELPP_STL_LOGGING
			ELPP_LOG_STD_ARRAY
			ELPP_LOG_UNORDERED_MAP
			ELPP_LOG_UNORDERED_SET
		)
  # target_sources(${target}
    # PRIVATE
      # ${EASYLOGGING_SOURCES}
  # )
  # target_include_directories(${target}
    # PRIVATE ${EASYLOGGING_INCLUDE_DIRS})
endfunction()

