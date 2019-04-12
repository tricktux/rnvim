# All the source files for the bot.
file(GLOB SOURCES "main.cpp" "application.cpp")

include_directories(${CMAKE_SOURCE_DIR}/inc)

add_executable(gnvim ${SOURCES})

# Link against libnvc
find_library(LIBNVC_LIBNAME nvc HINTS ${LIBNVC_LIB})
target_link_libraries(gnvim Threads::Threads)
target_link_libraries(gnvim nvc)
target_link_libraries(gnvim stdc++fs)

target_link_libraries(gnvim gflags)
target_link_libraries(gnvim reproc++)
target_link_libraries(gnvim glog)
target_link_libraries(gnvim sfml-system sfml-graphics)
