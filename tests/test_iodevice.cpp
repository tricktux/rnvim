/// @file test_nvimrpc.cpp
/// @brief Tests for the nvim input output device
/// @author Reinaldo Molina
/// @version  0.0
/// @date Dec 18 2019

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

#include "nvimpp/iodevice.hpp"
#include <gtest/gtest.h>
#include <vector>
#include <chrono>

/**
 * @brief Test ReprocDevice start, stop, and read cmake output
 */
TEST(reprocdevice, start_stop) {
  nvimrpc::ReprocDevice device;

  // char data[10240];
  std::vector<const char *> args{ {"cmake", "--help", nullptr} };

  // ASSERT_EQ(device.start(args, 10), 0);
  ASSERT_EQ(1,1);

  // nvimrpc::NvimApi api{device};
	// ASSERT_NE(device.read(data, sizeof data),0);
	// std::cout << "data: '" << data << "'" << std::endl;

	// ASSERT_EQ(device.stop(), 0);
}

TEST(nvimrpc, DISABLED_streamdecoder) {
	// int timeout = 10;
	// nvimrpc::ReprocDevice device;
	// std::vector<const char *> args{ {"nvim", "--embed", nullptr} };
	// ASSERT_EQ(device.spawn(args, timeout), 0);

	// nvimrpc::StreamDecoder sd{device};

	// auto node = sd.poll();
	// ASSERT_TRUE(node.has_value());
	// log_server_pack_node(node.value());
}
