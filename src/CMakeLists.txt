# All the source files for the bot.
file(GLOB SOURCES
	"${CMAKE_SOURCE_DIR}/src/main.cpp"
	"${CMAKE_SOURCE_DIR}/src/application.cpp"
	"${CMAKE_SOURCE_DIR}/src/process_handler.cpp"
	"${CMAKE_SOURCE_DIR}/src/cli_args.cpp"
)

include_directories(${CMAKE_SOURCE_DIR}/inc)
include_directories(${wxWidgets_INCLUDE_DIRS})

if (EASYLOGGING_FOUND)
	include_directories(${EASYLOGGING_INCLUDE_DIRS})
endif ()
if (LIBNVC_FOUND)
	include_directories(${LIBNVC_INCLUDE_DIRS})
endif ()
if (CXXOPTS_FOUND)
	include_directories(${CXXOPTS_INCLUDE_DIRS})
endif ()
if (REPROC++_FOUND)
	include_directories(${REPROC++_INCLUDE_DIRS})
endif ()

add_executable(${PROJECT_NAME} ${SOURCES})
# Sanitizers
if (SANITIZERS_FOUND)
	add_sanitizers(aws-text)
endif ()


if (EASYLOGGING_FOUND)
	target_link_libraries(gnvim ${EASYLOGGING_LIBRARIES})
endif ()
if (LIBNVC_FOUND)
	target_link_libraries(gnvim ${LIBNVC_LIBRARIES})
endif ()
if (REPROC++_FOUND)
	target_link_libraries(gnvim ${REPROC++_LIBRARIES})
endif ()
target_link_libraries(aws-text ${wxWidgets_LIBRARIES})

# target_link_libraries(gnvim stdc++fs)
# target_link_libraries(gnvim sfml-system sfml-graphics)
