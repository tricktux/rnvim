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

#include <gtest/gtest.h>
#include <vector>
#include <chrono>
#include "easylogging++.h"
#include "rpc/iodevice.hpp"

INITIALIZE_EASYLOGGINGPP

/**
 * @brief Test ReprocDevice spawn, recv, and "kill" process
 */
TEST(reprocdevice, start_stop) {
  nvimrpc::ReprocDevice device;

  const size_t data_size = 4096;
	std::vector<uint8_t> all_data;
	all_data.reserve(data_size*10);
	size_t data_read = 0;
  std::array<uint8_t, data_size> data{};
  std::vector<const char *> args{{"cmake", "--help", nullptr}};

  ASSERT_EQ(device.start(args, 10), 0);
	// for (int k = 0; k < 2; k++) {
		// data_read += device.read(data.data(), data_size);
		// all_data.insert(std::end(all_data), std::begin(data), std::end(data));
	// }
	while((data_read += device.read(data.data(), data_size)) == data_size) {
		all_data.insert(std::end(all_data), data);
		// std::this_thread::sleep_for(std::chrono::seconds{1});
	}
  ASSERT_GE(data_read, 0);
	ASSERT_FALSE(all_data.empty());
  std::cout << "data_size: '" << data_read << "'" << std::endl;
	std::cout << "data: '" << all_data.data() << "'" << std::endl;

  ASSERT_EQ(device.stop(), 0);
}

