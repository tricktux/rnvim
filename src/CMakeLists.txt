# All the source files for the bot.
file(GLOB SOURCES
		"${CMAKE_SOURCE_DIR}/src/main.cpp"
		"${CMAKE_SOURCE_DIR}/src/rpc/iodevice.cpp"
		# "${CMAKE_SOURCE_DIR}/src/rpc/streamdecoder.cpp"
		# "${CMAKE_SOURCE_DIR}/src/process_handler.cpp"
		# "${CMAKE_SOURCE_DIR}/src/cli_args.cpp"
	)

file(GLOB MPACK_SRCS
		"${CMAKE_SOURCE_DIR}/third-party/mpack/mpack.c"
	)

# Includes
include_directories(${CMAKE_SOURCE_DIR}/inc)
include_directories("${CMAKE_SOURCE_DIR}/third-party/mpack")
# include_directories(${JINJA2_OUTPUT_DIR})

if (EASYLOGGING_FOUND)
	include_directories(${EASYLOGGING_INCLUDE_DIRS})
endif ()
if (NVC_FOUND)
	include_directories(${NVC_INCLUDE_DIRS})
endif ()
if (CXXOPTS_FOUND)
	include_directories(${CXXOPTS_INCLUDE_DIRS})
endif ()
if (REPROC_FOUND)
	include_directories(${REPROC_INCLUDE_DIRS})
endif ()
if (GSL_FOUND)
	include_directories(SYSTEM ${GSL_INCLUDE_DIRS})
endif ()

# Executables
add_executable(${PROJECT_NAME} ${SOURCES} ${MPACK_SRCS})
# Dependencies
if (GSL_FOUND)
	add_dependencies(${PROJECT_NAME} gsl)
endif ()
if (EASYLOGGING_FOUND)
	add_dependencies(${PROJECT_NAME} easylogging)
endif ()
if (NVC_FOUND)
	add_dependencies(${PROJECT_NAME} nvc)
endif ()
if (CXXOPTS_FOUND)
	add_dependencies(${PROJECT_NAME} cxxopts)
endif ()
if (REPROC_FOUND)
	add_dependencies(${PROJECT_NAME} reproc)
endif ()
# add_dependencies(${PROJECT_NAME} jinja2_proc)

# Sanitizers
if (SANITIZERS_FOUND)
	add_sanitizers(${PROJECT_NAME})
endif ()

if (EASYLOGGING_FOUND)
	target_link_libraries(${PROJECT_NAME} ${EASYLOGGING_LIBRARIES})
endif ()
if (NVC_FOUND)
	target_link_libraries(${PROJECT_NAME} ${NVC_LIBRARIES})
endif ()
if (REPROC_FOUND)
	target_link_libraries(${PROJECT_NAME} ${REPROC_LIBRARIES})
endif ()
target_link_libraries(${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(${PROJECT_NAME} fmt::fmt)
