
# include("${CMAKE_SOURCE_DIR}/cmake/jinja2.cmake")
include("${CMAKE_SOURCE_DIR}/cmake/external/reproc++.cmake")
include("${CMAKE_SOURCE_DIR}/cmake/external/mpack.cmake")

set(LIB_NAME "nvimrpc")
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package (Threads)

file(GLOB LIB_SRC
	"${CMAKE_SOURCE_DIR}/lib/nvimrpc.cpp"
	"${MPACK_SRC}/mpack.c"
	)

add_library(nvimrpc STATIC ${JINJA2_SRC} ${LIB_SRC})
if (EASYLOGGING_FOUND)
	add_dependencies(nvimrpc easylogging)
	target_include_directories(nvimrpc PUBLIC ${EASYLOGGING_INCLUDE_DIRS})
	target_link_libraries(nvimrpc PUBLIC ${EASYLOGGING_LIBRARIES})
endif ()
if (REPROC_FOUND)
	add_dependencies(nvimrpc reproc)
	target_include_directories(nvimrpc SYSTEM PUBLIC ${REPROC_INCLUDE_DIRS})
	target_link_libraries(nvimrpc PUBLIC ${REPROC_LIBRARIES})
endif ()
if (MPACK_FOUND)
	add_dependencies(nvimrpc mpack)
	target_include_directories(nvimrpc PUBLIC ${MPACK_SRC})
endif()
# add_dependencies(nvimrpc jinja2_proc)

# target_include_directories(nvimrpc PUBLIC "${MPACK_INCLUDE_DIRS}")
# target_include_directories(nvimrpc PUBLIC "${JINJA2_OUTPUT_DIR}")
# endif ()

set(NVIMRPC_FOUND TRUE)
set(NVIMRPC_INCLUDE_DIRS ${JINJA2_OUTPUT_DIR} "${CMAKE_SOURCE_DIR}/lib")
set(NVIMRPC_LIBRARIES 
	${CMAKE_STATIC_LIBRARY_PREFIX}nvimrpc${CMAKE_STATIC_LIBRARY_SUFFIX}
	${CMAKE_THREAD_LIBS_INIT})
set(NVIMRPC_LIBRARY_DIRS ${EXECUTABLE_OUTPUT_PATH})

include (GNUInstallDirs)
install(TARGETS nvimrpc DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(FILES "${CMAKE_SOURCE_DIR}/lib/nvimrpc.hpp" DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
