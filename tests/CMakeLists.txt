include_directories(${CMAKE_SOURCE_DIR}/inc)

include_directories(SYSTEM "${GTEST_INCLUDE_DIRS}")

add_executable(test_process_handler test_process_handler.cpp
	"${CMAKE_SOURCE_DIR}/src/process_handler.cpp")

# Link test executable against gtest & gtest_main
target_link_libraries(test_process_handler reproc++)
target_link_libraries(test_process_handler ${EASYLOGGING_LIBNAME})
target_link_libraries(test_process_handler ${GTEST_BOTH_LIBRARIES})
target_link_libraries(test_process_handler Threads::Threads)

add_test(test_process_handler ${EXECUTABLE_OUTPUT_PATH})

# New test
# add_executable(
		# test_coordinator
		# test_coordinator.cpp
		# ${CMAKE_SOURCE_DIR}/src/config.cpp
		# ${CMAKE_SOURCE_DIR}/src/coordinator.cpp
		# ${CMAKE_SOURCE_DIR}/src/super_bot.cpp
	# )

# # Link test executable against gtest & gtest_main
# target_link_libraries(test_coordinator
		# ${GTEST_BOTH_LIBRARIES}
		# ${GLOG_LIBRARIES}
		# ${SC2Api_LIBRARIES}
		# pthread
		# dl
	# )

# add_test(test_coordinator ${EXECUTABLE_OUTPUT_PATH})
