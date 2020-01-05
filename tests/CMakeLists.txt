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

# ------------iodevice--------------
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

# ------------nvimapi--------------
add_executable(test_nvimapi ${MPACK_SRCS}
	"${CMAKE_SOURCE_DIR}/src/rpc/iodevice.cpp"
	"${CMAKE_SOURCE_DIR}/src/rpc/streamdecoder.cpp"
	"${CMAKE_SOURCE_DIR}/src/rpc/msgpack.cpp"
	# "${CMAKE_SOURCE_DIR}/src/rpc/template/nvimapi.cpp"
	"${CMAKE_SOURCE_DIR}/tests/test_nvimapi.cpp"
	)
if (REPROC_FOUND)
	add_dependencies(test_nvimapi reproc)
	target_include_directories(test_nvimapi PUBLIC SYSTEM ${REPROC_INCLUDE_DIRS})
	target_link_libraries(test_nvimapi PRIVATE ${REPROC_LIBRARIES})
endif ()
if (EASYLOGGING_FOUND)
	add_dependencies(test_nvimapi easylogging)
endif ()
if (JINJA2_FOUND)
	target_include_directories(test_nvimapi PUBLIC ${JINJA2_OUTPUT_DIR})
	add_dependencies(test_nvimapi jinja2_proc)
endif ()
add_test(test_nvimapi ${EXECUTABLE_OUTPUT_PATH})

# ------------mpack--------------
add_executable(test_mpack ${MPACK_SRCS}
	"${CMAKE_SOURCE_DIR}/src/rpc/msgpack.cpp"
	"${CMAKE_SOURCE_DIR}/tests/test_mpack.cpp"
	)
if (EASYLOGGING_FOUND)
	add_dependencies(test_mpack easylogging)
endif ()
if (JINJA2_FOUND)
	target_include_directories(test_mpack PUBLIC ${JINJA2_OUTPUT_DIR})
	add_dependencies(test_mpack jinja2_proc)
endif ()
add_test(test_mpack ${EXECUTABLE_OUTPUT_PATH})

# ------------libnvc--------------
if (ENABLE_LIBNVC_TESTS)
	include("${CMAKE_SOURCE_DIR}/cmake/external/libnvc.cmake")
	add_executable(test_nvc
		# ${MPACK_SRCS}
		# "${CMAKE_SOURCE_DIR}/src/rpc/iodevice.cpp"
		# "${CMAKE_SOURCE_DIR}/src/rpc/msgpack.cpp"
		# "${CMAKE_SOURCE_DIR}/src/rpc/template/nvimapi.cpp"
		"${CMAKE_SOURCE_DIR}/tests/test_nvc.cpp"
		)
	if (NVC_FOUND)
		add_dependencies(test_nvc nvc)
		target_include_directories(test_nvc PUBLIC ${NVC_INCLUDE_DIRS})
		target_link_libraries(test_nvc PRIVATE ${NVC_LIBRARIES})
	endif ()
	add_test(test_nvc ${EXECUTABLE_OUTPUT_PATH})
endif ()

