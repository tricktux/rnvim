include_directories(${CMAKE_SOURCE_DIR}/inc)

include_directories(SYSTEM "${GTEST_INCLUDE_DIRS}")
link_libraries(${EASYLOGGING_LIBNAME})
link_libraries(${GTEST_BOTH_LIBRARIES})
link_libraries(Threads::Threads)

add_executable(test_process_handler test_process_handler.cpp
	"${CMAKE_SOURCE_DIR}/src/process_handler.cpp")

# Link test executable against gtest & gtest_main
target_link_libraries(test_process_handler reproc++)

add_test(test_process_handler ${EXECUTABLE_OUTPUT_PATH})

# New test
add_executable(test_cli_args test_cli_args.cpp
	"${CMAKE_SOURCE_DIR}/src/cli_args.cpp")

# Link test executable against gtest & gtest_main
target_link_libraries(test_cli_args cxxopts)

add_test(test_cli_args ${EXECUTABLE_OUTPUT_PATH})
