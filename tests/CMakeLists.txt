include_directories(${CMAKE_SOURCE_DIR}/inc)

if (GTEST_FOUND)
	include_directories(SYSTEM "${GTEST_INCLUDE_DIRS}")
	link_libraries(${GTEST_BOTH_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
endif()

if (EASYLOGGING_FOUND)
	include_directories(${EASYLOGGING_INCLUDE_DIRS})
	link_libraries(${EASYLOGGING_LIBRARIES})
endif()

# if (REPROC_FOUND)
	# include_directories(SYSTEM ${REPROC_INCLUDE_DIRS})
	# link_libraries(${REPROC_LIBRARIES})
# endif ()

# add_executable(test_process_handler test_process_handler.cpp
	# "${CMAKE_SOURCE_DIR}/src/process_handler.cpp")

# # Link test executable against gtest & gtest_main
# target_link_libraries(test_process_handler reproc++)

# add_test(test_process_handler ${EXECUTABLE_OUTPUT_PATH})

# # New test
# add_executable(test_cli_args test_cli_args.cpp
	# "${CMAKE_SOURCE_DIR}/src/cli_args.cpp")

# # Link test executable against gtest & gtest_main
# target_link_libraries(test_cli_args cxxopts)

# add_test(test_cli_args ${EXECUTABLE_OUTPUT_PATH})

# ------------nvimrpc--------------
add_executable(test_nvimrpc
		"${CMAKE_SOURCE_DIR}/src/rpc/nvimrpc.cpp"
		"${CMAKE_SOURCE_DIR}/tests/test_nvimrpc.cpp"
	)
if (REPROC_FOUND)
	add_dependencies(test_nvimrpc reproc)
	target_include_directories(test_nvimrpc PUBLIC SYSTEM ${REPROC_INCLUDE_DIRS})
	target_link_libraries(test_nvimrpc PRIVATE ${REPROC_LIBRARIES})
endif ()
if (EASYLOGGING_FOUND)
	add_dependencies(test_nvimrpc easylogging)
endif ()

add_test(test_nvimrpc ${EXECUTABLE_OUTPUT_PATH})

# if (MPACK_FOUND)
	# add_dependencies(test_nvimrpc mpack)
	# target_include_directories(test_nvimrpc PUBLIC ${MPACK_SRC})
# endif()
