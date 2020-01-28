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
    BUILD_IN_SOURCE 1
    CONFIGURE_COMMAND ""
    BUILD_COMMAND make nvim -j${NPROC} CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
                      CMAKE_INSTALL_PREFIX=${nvim_INSTALL}
    INSTALL_COMMAND make install
    LOG_DOWNLOAD TRUE
    LOG_BUILD TRUE
    LOG_INSTALL TRUE
  )

set(NVIM_FOUND TRUE)
set(NVIM_EXE ${nvim_INSTALL}/bin/nvim)
set(NVIM_EXTERNAL TRUE)
