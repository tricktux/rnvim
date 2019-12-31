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

INITIALIZE_EASYLOGGINGPP

TEST(api, simple) {
  int timeout = 10;
  nvimrpc::ReprocDevice device;
	std::vector<const char *> args{{"nvim", "-u", "NONE", "--embed", nullptr}};
	ASSERT_EQ(device.spawn(args, timeout), 0);

	nvimrpc::NvimApi api{device};
	api.nvim_ui_attach(800, 600, { { "rgb", true } });
	api.nvim_input("$i123<CR>123<ESC>");
	api.nvim_buf_set_name(0, ":D");
	long lines = api.nvim_buf_line_count(0);
	ASSERT_EQ(lines, 2);
}
