
set(JINJA2_FOUND TRUE)
set(JINJA2_INPUT_DIR  "${CMAKE_SOURCE_DIR}/inc/rpc/template")
set(JINJA2_OUTPUT_DIR "${CMAKE_BINARY_DIR}/generated")

file(GLOB JINJA2_ALL_TEMPLATE_SRC
		"${JINJA2_INPUT_DIR}/nvimapi.hpp"
	)

set(JINJA2_SRC "")
foreach(JINJA2_CURR_TEMPLATE_SRC ${JINJA2_ALL_TEMPLATE_SRC})
	message(STATUS "jinja_src += ${JINJA2_CURR_TEMPLATE_SRC}")
	get_filename_component(JINJA2_CURR_SRC ${JINJA2_CURR_TEMPLATE_SRC} NAME)
	set(JINJA2_SRC ${JINJA2_SRC} ${JINJA2_OUTPUT_DIR}/${JINJA2_CURR_SRC})
endforeach()

set(JINJA2_SCRIPT_NAME nvim_api.py)
set(JINJA2_SCRIPT_DIR ${CMAKE_SOURCE_DIR}/scripts)
add_custom_command(OUTPUT ${JINJA2_SRC}
	COMMAND python3 ${JINJA2_SCRIPT_NAME} -t ${JINJA2_INPUT_DIR} -o ${JINJA2_OUTPUT_DIR}
	COMMENT "Generate nvim server msgpack-rpc api..."
	VERBATIM 
	DEPENDS ${JINJA2_ALL_TEMPLATE_SRC}
	WORKING_DIRECTORY ${JINJA2_SCRIPT_DIR}
)

# Everywhere you want to include ${JINJA2_SRC}
# You must copy and paste this line :(.... ask CMake
set_source_files_properties(${JINJA2_SRC}
		PROPERTIES GENERATED 1
	)

add_custom_target(jinja2_proc DEPENDS ${JINJA2_SRC})
