include(ExternalProject)
include(ProcessorCount)
ProcessorCount(NPROC)
if(NPROC EQUAL 0)
  set(NPROC 1)
endif()

# build directory
set(nvim_PREFIX ${CMAKE_BINARY_DIR}/external/nvim-prefix)
# install directory
set(nvim_INSTALL ${CMAKE_BINARY_DIR}/external/nvim-install)

ExternalProject_Add(
    nvim
    PREFIX ${nvim_PREFIX}
    GIT_REPOSITORY https://github.com/neovim/neovim
    GIT_TAG				 v0.4.3
    GIT_SHALLOW		 TRUE
    GIT_PROGRESS	 TRUE
    UPDATE_DISCONNECTED TRUE
		CMAKE_ARGS  -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
                -DCMAKE_INSTALL_PREFIX=${fmt_INSTALL}
    LOG_DOWNLOAD TRUE
    LOG_INSTALL TRUE
  )

ExternalProject_Get_property(nvim SOURCE_DIR)
set(NVIM_FOUND TRUE)
set(NVIM_BIN ${SOURCE_DIR}/include)
