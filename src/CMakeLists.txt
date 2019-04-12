# All the source files for the bot.
file(GLOB SOURCES "main.cpp")

include_directories(${CMAKE_SOURCE_DIR}/inc)

# include_directories("${LIBNVC_INCLUDE}")

# Show more warnings at compiletime.
if (MSVC)
    # FIXME: put Windows specific options here.
else ()
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
endif ()

# Set ExecutableName.
add_executable(gnvim ${SOURCES})

# add_dependencies(gnvim libnvc)

# TODO - Link agins sfml-xxx

# Link against libnvc
find_library(LIBNVC_LIBNAME nvc HINTS ${LIBNVC_LIB})
target_link_libraries(gnvim Threads::Threads)
target_link_libraries(gnvim nvc)
target_link_libraries(gnvim stdc++fs)

target_link_libraries(gnvim gflags)
target_link_libraries(gnvim reproc++)
target_link_libraries(gnvim glog)
target_link_libraries(gnvim sfml-system sfml-graphics)

if (APPLE)
	target_link_libraries(gnvim "-framework Carbon")
endif ()

# Linux specific.
if (UNIX AND NOT APPLE)
	target_link_libraries(gnvim pthread dl)
endif ()
