include(ExternalProject)


ExternalProject_Add(
		mpack
		URL https://github.com/ludocode/mpack/releases/download/v1.0/mpack-amalgamation-1.0.tar.gz
		URL_HASH SHA256=25c2f22a30b5207288ffededc092c7d2cb5599cc0eea9d5787d738088429cd56
		UPDATE_COMMAND ""
		CONFIGURE_COMMAND ""
		BUILD_COMMAND ""
		INSTALL_COMMAND ""
		TEST_COMMAND ""
		LOG_DOWNLOAD TRUE
	)

ExternalProject_Get_property(mpack SOURCE_DIR)
set(MPACK_FOUND TRUE)
set(MPACK_SRC ${SOURCE_DIR}/src/mpack)
set(MPACK_EXTERNAL TRUE)
