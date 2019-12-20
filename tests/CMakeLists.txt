file(GLOB TEST_NVIMRPC_SRCS
		"${CMAKE_SOURCE_DIR}/src/rpc/iodevice.cpp"
		"${CMAKE_SOURCE_DIR}/src/rpc/streamdecoder.cpp"
		"${CMAKE_SOURCE_DIR}/tests/test_nvimrpc.cpp"
	)
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
add_executable(test_nvimrpc ${TEST_NVIMRPC_SRCS} ${MPACK_SRCS})
if (REPROC_FOUND)
	add_dependencies(test_nvimrpc reproc)
	target_include_directories(test_nvimrpc PUBLIC SYSTEM ${REPROC_INCLUDE_DIRS})
	target_link_libraries(test_nvimrpc PRIVATE ${REPROC_LIBRARIES})
endif ()
if (EASYLOGGING_FOUND)
	add_dependencies(test_nvimrpc easylogging)
endif ()
add_test(test_nvimrpc ${EXECUTABLE_OUTPUT_PATH})
