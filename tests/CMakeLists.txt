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
if (reprocxx_FOUND)
	include_directories(SYSTEM ${reprocxx_INCLUDE_DIRS})
	link_libraries(${reprocxx_LIBRARIES})
endif ()

set_source_files_properties(${JINJA2_SRC}
	PROPERTIES GENERATED 1
	)

# function(add_my_tests tests)
	# foreach(test ${tests})
		# message(STATUS "test = ${test}")
		# add_test(NAME ${test}
			# COMMAND ${test}
			# WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
			# )
	# endforeach()
# endfunction()

# set(TESTS "")

# ------------iodevice--------------
list(APPEND TESTS test_iodevice)
add_executable(test_iodevice "")
target_sources(test_iodevice
		PRIVATE
			test_iodevice.cpp
			${CMAKE_SOURCE_DIR}/src/rpc/iodevice.cpp
			${CMAKE_SOURCE_DIR}/src/rpc/msgpack.cpp
			${CMAKE_SOURCE_DIR}/inc/rpc/iodevice.hpp
			${CMAKE_SOURCE_DIR}/inc/rpc/msgpack.hpp
			${JINJA2_SRC}
	)
target_include_mpack(test_iodevice)
if (reprocxx_EXTERNAL)
	add_dependencies(test_iodevice reproc)
endif()
if (EASYLOGGING_FOUND)
	add_dependencies(test_iodevice easylogging)
	target_add_easylogging_definitions(test_iodevice)
endif ()
add_test(NAME test_iodevice
	COMMAND test_iodevice
	WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
)

# ------------nvimapi--------------
list(APPEND TESTS test_nvimapi)
add_executable(test_nvimapi "")
target_sources(test_nvimapi
		PRIVATE
			test_nvimapi.cpp
			${CMAKE_SOURCE_DIR}/src/rpc/iodevice.cpp
			${CMAKE_SOURCE_DIR}/src/rpc/msgpack.cpp
			${CMAKE_SOURCE_DIR}/inc/rpc/iodevice.hpp
			${CMAKE_SOURCE_DIR}/inc/rpc/msgpack.hpp
			${JINJA2_SRC}
	)
target_include_mpack(test_nvimapi)
if (reprocxx_EXTERNAL)
	add_dependencies(test_iodevice reproc)
endif()
if (EASYLOGGING_FOUND)
	add_dependencies(test_nvimapi easylogging)
	target_add_easylogging_definitions(test_nvimapi)
endif ()
if (JINJA2_FOUND)
	target_include_directories(test_nvimapi PRIVATE ${JINJA2_OUTPUT_DIR})
	add_dependencies(test_nvimapi jinja2_proc)
endif ()
add_test(NAME test_nvimapi
	COMMAND test_nvimapi
	WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
)

# ------------mpack--------------
list(APPEND TESTS test_mpack)
add_executable(test_mpack "")
target_sources(test_mpack
		PRIVATE
			test_mpack.cpp
			${CMAKE_SOURCE_DIR}/src/rpc/msgpack.cpp
			${CMAKE_SOURCE_DIR}/inc/rpc/msgpack.hpp
	)
target_include_mpack(test_mpack)
if (EASYLOGGING_FOUND)
	add_dependencies(test_mpack easylogging)
	target_add_easylogging_definitions(test_mpack)
endif ()
if (JINJA2_FOUND)
	target_include_directories(test_mpack PRIVATE ${JINJA2_OUTPUT_DIR})
	add_dependencies(test_mpack jinja2_proc)
endif ()
add_test(NAME test_mpack
	COMMAND test_mpack
	WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
)

# ------------libnvc--------------
if (ENABLE_LIBNVC_TESTS)
	include("${CMAKE_SOURCE_DIR}/cmake/external/libnvc.cmake")
	list(APPEND TESTS test_nvc)
	add_executable(test_nvc
			"${CMAKE_SOURCE_DIR}/tests/test_nvc.cpp"
		)
	if (NVC_FOUND)
		add_dependencies(test_nvc nvc)
		target_include_directories(test_nvc PRIVATE ${NVC_INCLUDE_DIRS})
		target_link_libraries(test_nvc PRIVATE ${NVC_LIBRARIES})
	endif ()
	add_test(NAME test_nvc
		COMMAND test_nvc
		WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
	)
endif ()

# message(STATUS "TESTS = ${TESTS}")
# add_my_tests(TESTS)

