# All the source files for the bot.
file(GLOB SOURCES
	"main.cpp"
	"application.cpp"
	"process_handler.cpp"
	"cli_args.cpp"
)

include_directories(${CMAKE_SOURCE_DIR}/inc)

add_executable(gnvim ${SOURCES})

# Link against libnvc
find_library(LIBNVC_LIBNAME nvc
	HINTS ${CMAKE_SOURCE_DIR}/lib/${CMAKE_BUILD_TYPE})
find_library(EASYLOGGING_LIBNAME easyloggingpp
	HINTS ${CMAKE_SOURCE_DIR}/lib/${CMAKE_BUILD_TYPE})

target_link_libraries(gnvim gflags)
target_link_libraries(gnvim reproc++)
target_link_libraries(gnvim ${EASYLOGGING_LIBNAME})
target_link_libraries(gnvim ${LIBNVC_LIBNAME})
target_link_libraries(gnvim Threads::Threads)
target_link_libraries(gnvim stdc++fs)
target_link_libraries(gnvim sfml-system sfml-graphics)
