/// @file test_nvimapi.cpp
/// @brief Tests for the nvim api
/// @author Reinaldo Molina
/// @version  0.0
/// @date Dec 31 2019
// Copyright © 2019 Reinaldo Molina

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "easylogging++.h"
#include "rpc/iodevice.hpp"
#include "rpc/msgpack.hpp"
#include "nvimapi.hpp"

#include <chrono>
#include <gtest/gtest.h>
#include <vector>
#include <filesystem>

namespace fs = std::filesystem;

INITIALIZE_EASYLOGGINGPP
int timeout = 10;
int size_x = 100;
int size_y = 100;
// std::vector<const char *> args{{"nvim", "-u", "NORC", "--embed", nullptr}};
std::vector<const char *> args{{"nvim", "--embed", nullptr}};

TEST(api, input) {
  nvimrpc::ReprocDevice device;
  ASSERT_EQ(device.spawn(args, timeout), 0);

  nvimrpc::NvimApi api{device};
  api.nvim_ui_attach(size_x, size_y, {{"rgb", true}});
  ASSERT_GT(api.nvim_input("$i123<CR>123<ESC>"), 0);
  device.kill();
}

TEST(api, buf_set_name) {
  nvimrpc::ReprocDevice device;
  ASSERT_EQ(device.spawn(args, timeout), 0);

  nvimrpc::NvimApi api{device};
  api.nvim_ui_attach(size_x, size_y, {{"rgb", true}});
  api.nvim_input("$i123<CR>123<ESC>");
  api.nvim_buf_set_name(1, ":D");
  device.kill();
}

TEST(api, buf_get_name) {
  std::string buf{"yixx"};
  nvimrpc::ReprocDevice device;
  ASSERT_EQ(device.spawn(args, timeout), 0);

  nvimrpc::NvimApi api{device};
  api.nvim_ui_attach(size_x, size_y, {{"rgb", true}});
  api.nvim_input("$i123<CR>123<ESC>");
  api.nvim_buf_set_name(1, buf);
  std::string name = api.nvim_buf_get_name(1);
  ASSERT_TRUE(!name.empty());
  device.kill();
}

int main(int argc, char *argv[]) {
	fs::path elconf{fs::current_path()};
	if (elconf.empty()) {
		std::cout << "Failed to get fs::current_path" << std::endl;
		exit(8);
	}

	elconf.append("easylogingpp.conf");
	if (!fs::is_regular_file(elconf)) {
		std::cout << "Easylogging configuration does not exist" << std::endl;
		exit(9);
	}
	START_EASYLOGGINGPP(argc, argv);
	testing::InitGoogleTest(&argc, argv);
	el::Configurations conf(elconf.c_str());
	// Actually reconfigure all loggers instead
	el::Loggers::reconfigureAllLoggers(conf);
	// Now all the loggers will use configuration from file
	DLOG(INFO) << ">>>>Start of Log<<<<";
	DLOG(INFO) << "Easylogging config file: '" << elconf << "'";
  return RUN_ALL_TESTS();
}
