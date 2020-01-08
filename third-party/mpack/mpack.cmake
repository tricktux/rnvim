
function(target_include_mpack target)
	target_compile_definitions(${target}
			PRIVATE
				MPACK_DEBUG
				MPACK_STDIO
				MPACK_STRINGS
				MPACK_EXTENSIONS
				MPACK_READ_TRACKING
				MPACK_WRITE_TRACKING
		)
	target_sources(${target}
			PRIVATE
				${CMAKE_SOURCE_DIR}/third-party/mpack/mpack.c
				${CMAKE_SOURCE_DIR}/third-party/mpack/mpack.h
		)
endfunction()
