
# include("${CMAKE_SOURCE_DIR}/cmake/jinja2.cmake")
include("${CMAKE_SOURCE_DIR}/cmake/external/reproc++.cmake")

set(LIB_NAME "nvim")
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package (Threads)

file(GLOB LIB_SRC
	"${CMAKE_SOURCE_DIR}/lib/nvim.cpp"
	# "${CMAKE_SOURCE_DIR}/lib/mpinterf.cpp"
	)

add_library(${LIB_NAME} STATIC ${JINJA2_SRC} ${LIB_SRC})
if (EASYLOGGING_FOUND)
	add_dependencies(${LIB_NAME} easylogging)
	target_include_directories(${LIB_NAME} PUBLIC ${EASYLOGGING_INCLUDE_DIRS})
	target_link_libraries(${LIB_NAME} PRIVATE ${EASYLOGGING_LIBRARIES})
endif ()
if (REPROC_FOUND)
	add_dependencies(${LIB_NAME} reproc)
	target_include_directories(${LIB_NAME} PUBLIC ${REPROC_INCLUDE_DIRS})
	target_link_libraries(${LIB_NAME} PRIVATE ${REPROC_LIBRARIES})
endif ()
# add_dependencies(${LIB_NAME} jinja2_proc)

# target_include_directories(${LIB_NAME} PUBLIC "${MPACK_INCLUDE_DIRS}")
# target_include_directories(${LIB_NAME} PUBLIC "${JINJA2_OUTPUT_DIR}")
# endif ()

set(NVC_FOUND TRUE)
set(NVC_INCLUDE_DIRS ${JINJA2_OUTPUT_DIR})
set(NVC_LIBRARIES 
	${CMAKE_STATIC_LIBRARY_PREFIX}${LIB_NAME}${CMAKE_STATIC_LIBRARY_SUFFIX}
	${CMAKE_THREAD_LIBS_INIT})
set(NVC_LIBRARY_DIRS ${EXECUTABLE_OUTPUT_PATH})

# install(TARGETS ${LIB_NAME} DESTINATION lib)
# install(FILES ${JINJA2_OUTPUT_DIR}/libnvc.hpp DESTINATION include)
