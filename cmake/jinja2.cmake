
set(JINJA2_INPUT_DIR  "${CMAKE_SOURCE_DIR}/lib")
set(JINJA2_OUTPUT_DIR "${PROJECT_BINARY_DIR}/lib/generated")

file(GLOB JINJA2_ALL_TEMPLATE_SRC
		"${JINJA2_INPUT_DIR}/libnvc.hpp"
		"${JINJA2_INPUT_DIR}/apiclient.cpp"
		"${JINJA2_INPUT_DIR}/reprocdevice.cpp"
	)

set(JINJA2_SRC "")
foreach(JINJA2_CURR_TEMPLATE_SRC ${JINJA2_ALL_TEMPLATE_SRC})
	message(STATUS "jinja_src += ${JINJA2_CURR_TEMPLATE_SRC}")
	get_filename_component(JINJA2_CURR_SRC ${JINJA2_CURR_TEMPLATE_SRC} NAME)
	set(JINJA2_SRC ${JINJA2_SRC} ${JINJA2_OUTPUT_DIR}/${JINJA2_CURR_SRC})
endforeach()

set(JINJA2_SCRIPT_LOC ${CMAKE_SOURCE_DIR}/scripts/nvim_api.py)
add_custom_command(OUTPUT ${JINJA2_SRC}
	COMMAND python3 ${JINJA2_SCRIPT_LOC} -t ${JINJA2_INPUT_DIR} -o ${JINJA2_OUTPUT_DIR}
	COMMENT "Generate nvim server msgpack-rpc api..."
	VERBATIM 
)

add_custom_target(jinja2_proc DEPENDS ${JINJA2_SRC})
