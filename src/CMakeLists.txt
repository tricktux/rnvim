
set_source_files_properties(${JINJA2_SRC}
		PROPERTIES GENERATED 1
	)

# Executables
add_executable(${PROJECT_NAME} "")

# Add code coverage
# As an executable target, the reports will exclude the non-covered.cpp file,
# and any files in a test/ folder.
target_code_coverage(${PROJECT_NAME}
	EXCLUDE ${CMAKE_SOURCE_DIR}/third-party/mpack/*
	)

target_sources(${PROJECT_NAME}
		PRIVATE
			main.cpp
			rpc/iodevice.cpp
			rpc/msgpack.cpp
			${CMAKE_SOURCE_DIR}/inc/rpc/iodevice.hpp
			${CMAKE_SOURCE_DIR}/inc/rpc/msgpack.hpp
			${JINJA2_SRC}
	)
target_include_mpack(${PROJECT_NAME})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/inc)
target_include_directories(${PROJECT_NAME}
	PRIVATE ${CMAKE_SOURCE_DIR}/third-party/mpack)
target_link_libraries(${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(${PROJECT_NAME} fmt::fmt)

if (JINJA2_FOUND)
	add_dependencies(${PROJECT_NAME} jinja2_proc)
	target_include_directories(${PROJECT_NAME} PRIVATE ${JINJA2_OUTPUT_DIR})
endif ()
if (EASYLOGGING_FOUND)
	add_dependencies(${PROJECT_NAME} easylogging)
	target_add_easylogging_definitions(${PROJECT_NAME})
	target_include_directories(${PROJECT_NAME} PRIVATE ${EASYLOGGING_INCLUDE_DIRS})
	target_link_libraries(${PROJECT_NAME} ${EASYLOGGING_LIBRARIES})
endif ()
if (NVC_FOUND)
	add_dependencies(${PROJECT_NAME} nvc)
	target_include_directories(${PROJECT_NAME} PRIVATE ${NVC_INCLUDE_DIRS})
	target_link_libraries(${PROJECT_NAME} ${NVC_LIBRARIES})
endif ()
if (CXXOPTS_FOUND)
	add_dependencies(${PROJECT_NAME} cxxopts)
	target_include_directories(${PROJECT_NAME} PRIVATE ${CXXOPTS_INCLUDE_DIRS})
endif ()
if (reprocxx_FOUND)
	if (reprocxx_EXTERNAL)
		add_dependencies(${PROJECT_NAME} reproc)
	endif ()
	target_include_directories(${PROJECT_NAME} PRIVATE SYSTEM ${reprocxx_INCLUDE_DIRS})
	target_link_libraries(${PROJECT_NAME} ${reprocxx_LIBRARIES})
endif ()
if (GSL_FOUND)
	add_dependencies(${PROJECT_NAME} gsl)
	target_include_directories(${PROJECT_NAME} PRIVATE SYSTEM ${GSL_INCLUDE_DIRS})
endif ()

# Sanitizers
if (SANITIZERS_FOUND)
	add_sanitizers(${PROJECT_NAME})
endif ()

