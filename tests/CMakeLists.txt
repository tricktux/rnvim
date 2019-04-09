include_directories(${CMAKE_SOURCE_DIR}/inc)

include_directories(SYSTEM
		"${RAPIDJSON_INCLUDE_DIRS}"
		"${GLOG_INCLUDE_DIRS}"
		"${GTEST_INCLUDE_DIRS}"
		"${SC2Api_INCLUDE_DIRS}"
	)

# Copy test configuration file to output
file(
		COPY ${CMAKE_SOURCE_DIR}/config/
		DESTINATION ${EXECUTABLE_OUTPUT_PATH}
		FILES_MATCHING PATTERN "*.json"
	)

# TODO-[RM]-(Thu Aug 16 2018 06:02):
# - Make this a function. That you call in a loop
# Add executable and list sources to the executable
add_executable(test_config test_config.cpp ${CMAKE_SOURCE_DIR}/src/config.cpp)

# Link test executable against gtest & gtest_main
target_link_libraries(test_config
		${GTEST_BOTH_LIBRARIES}
		${GLOG_LIBRARIES}
	)

add_test(test_config ${EXECUTABLE_OUTPUT_PATH})

add_executable(
		test_coordinator 
		test_coordinator.cpp 
		${CMAKE_SOURCE_DIR}/src/config.cpp
		${CMAKE_SOURCE_DIR}/src/coordinator.cpp
		${CMAKE_SOURCE_DIR}/src/super_bot.cpp
	)

# Link test executable against gtest & gtest_main
target_link_libraries(test_coordinator
		${GTEST_BOTH_LIBRARIES}
		${GLOG_LIBRARIES}
		${SC2Api_LIBRARIES}
		pthread
		dl
	)

add_test(test_coordinator ${EXECUTABLE_OUTPUT_PATH})
