file(GLOB MPACK_SRCS
		"${CMAKE_SOURCE_DIR}/third-party/mpack/mpack.c"
	)

include_directories(${CMAKE_SOURCE_DIR}/inc)
include_directories("${CMAKE_SOURCE_DIR}/third-party/mpack")

if (GTEST_FOUND)
	include_directories(SYSTEM "${GTEST_INCLUDE_DIRS}")
	link_libraries(${GTEST_BOTH_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
endif()

if (EASYLOGGING_FOUND)
	include_directories(${EASYLOGGING_INCLUDE_DIRS})
	link_libraries(${EASYLOGGING_LIBRARIES})
endif()

# ------------nvimrpc--------------
add_executable(test_iodevice ${MPACK_SRCS}
		"${CMAKE_SOURCE_DIR}/src/rpc/iodevice.cpp"
		"${CMAKE_SOURCE_DIR}/tests/test_iodevice.cpp"
	)
if (REPROC_FOUND)
	add_dependencies(test_iodevice reproc)
	target_include_directories(test_iodevice PUBLIC SYSTEM ${REPROC_INCLUDE_DIRS})
	target_link_libraries(test_iodevice PRIVATE ${REPROC_LIBRARIES})
endif ()
if (EASYLOGGING_FOUND)
	add_dependencies(test_iodevice easylogging)
endif ()
add_test(test_iodevice ${EXECUTABLE_OUTPUT_PATH})
