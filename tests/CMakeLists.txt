aux_source_directory(${MPACK_SRC} MPACK_SRCS)

file(GLOB TEST_NVIMRPC_SRCS
		"${CMAKE_SOURCE_DIR}/src/rpc/iodevice.cpp"
		"${CMAKE_SOURCE_DIR}/src/rpc/streamdecoder.cpp"
		"${CMAKE_SOURCE_DIR}/tests/test_nvimrpc.cpp"
	)
include_directories(${CMAKE_SOURCE_DIR}/inc)

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
# add_dependencies(test_nvimrpc ${PROJECT_NAME})
if (MPACK_FOUND)
	message(STATUS "MPACK FOUND!!!")
	message(STATUS "MPACK_SRCS = ${MPACK_SRCS}")
	message(STATUS "MPACK_SRC = ${MPACK_SRC}")
	add_dependencies(test_nvimrpc mpack)
	target_include_directories(test_nvimrpc PUBLIC ${MPACK_SRC})
endif()
if (REPROC_FOUND)
	add_dependencies(test_nvimrpc reproc)
	target_include_directories(test_nvimrpc PUBLIC SYSTEM ${REPROC_INCLUDE_DIRS})
	target_link_libraries(test_nvimrpc PRIVATE ${REPROC_LIBRARIES})
endif ()
if (EASYLOGGING_FOUND)
	add_dependencies(test_nvimrpc easylogging)
endif ()
# add_test(test_nvimrpc ${EXECUTABLE_OUTPUT_PATH})
