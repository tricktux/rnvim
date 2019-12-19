include(ExternalProject)


ExternalProject_Add(
		mpack
		URL https://github.com/ludocode/mpack/releases/download/v1.0/mpack-amalgamation-1.0.tar.gz
		URL_HASH SHA256=25c2f22a30b5207288ffededc092c7d2cb5599cc0eea9d5787d738088429cd56
		# UPDATE_COMMAND ""
		# UPDATE_DISCONNECTED TRUE
		CONFIGURE_COMMAND ""
		BUILD_COMMAND ""
		INSTALL_COMMAND ""
		LOG_DOWNLOAD TRUE
	)

ADD_DEFINITIONS(-DMPACK_DEBUG)
ADD_DEFINITIONS(-DMPACK_STDIO)
ADD_DEFINITIONS(-DMPACK_STRINGS)
ADD_DEFINITIONS(-DMPACK_EXTENSIONS)
ADD_DEFINITIONS(-DMPACK_READ_TRACKING)
ADD_DEFINITIONS(-DMPACK_WRITE_TRACKING)

ExternalProject_Add_StepTargets(mpack done)
ExternalProject_Get_property(mpack SOURCE_DIR)
set(MPACK_FOUND TRUE)
set(MPACK_SRC ${SOURCE_DIR}/src/mpack)
# set(MPACK_SRCS "${SOURCE_DIR}/src/mpack/mpack.c")
set(MPACK_EXTERNAL TRUE)

# add_custom_target(mpack_downloaded DEPENDS mpack)
