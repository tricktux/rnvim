
include("${CMAKE_SOURCE_DIR}/cmake/jinja2.cmake")
include("${CMAKE_SOURCE_DIR}/cmake/external/reproc++.cmake")

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package (Threads)

file(GLOB LIB_SRC
	# "${CMAKE_SOURCE_DIR}/lib/reprocdevice.cpp"
	# "${CMAKE_SOURCE_DIR}/lib/mpinterf.cpp"
	)

add_library(nvc STATIC ${JINJA2_SRC} ${LIB_SRC})
if (EASYLOGGING_FOUND)
	add_dependencies(nvc easylogging)
	target_include_directories(nvc PUBLIC ${EASYLOGGING_INCLUDE_DIRS})
	target_link_libraries(nvc PRIVATE ${EASYLOGGING_LIBRARIES})
endif ()
if (REPROC_FOUND)
	add_dependencies(nvc reproc)
	target_include_directories(nvc PUBLIC ${REPROC_INCLUDE_DIRS})
	target_link_libraries(nvc PRIVATE ${REPROC_LIBRARIES})
endif ()
add_dependencies(nvc jinja2_proc)
target_link_libraries(nvc PRIVATE reproc++)

# target_include_directories(nvc PUBLIC "${MPACK_INCLUDE_DIRS}")
target_include_directories(nvc PUBLIC "${JINJA2_OUTPUT_DIR}")
# endif ()

set(NVC_FOUND TRUE)
set(NVC_INCLUDE_DIRS ${JINJA2_OUTPUT_DIR})
set(NVC_LIBRARIES 
	${CMAKE_STATIC_LIBRARY_PREFIX}nvc${CMAKE_STATIC_LIBRARY_SUFFIX}
	${CMAKE_THREAD_LIBS_INIT})
set(NVC_LIBRARY_DIRS ${EXECUTABLE_OUTPUT_PATH})

# install(TARGETS nvc DESTINATION lib)
# install(FILES ${JINJA2_OUTPUT_DIR}/libnvc.hpp DESTINATION include)
