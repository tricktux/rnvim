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
 * @brief Test ReprocDevice start, stop, and read cmake output
 */
TEST(reprocdevice, start_stop) {
  nvimrpc::ReprocDevice device;

  const size_t data_size = 4096;
  std::vector<uint8_t> all_data;
  all_data.reserve(data_size * 10);
  size_t data_read = 0, total_data_read = 0;
  std::array<uint8_t, data_size> data{};
  std::vector<const char *> args{{"cmake", "--help", nullptr}};

  ASSERT_EQ(device.start(args, 10), 0);
  // Read only 2 times, since that is all it takes to read output of cmake
  // --help
  for (int k = 0; k < 2; k++) {
    data_read = device.read(data.data(), data_size);
    total_data_read += data_read;
    all_data.insert(std::end(all_data), data.begin(), data.begin() + data_read);
  }

  ASSERT_EQ(total_data_read, all_data.size());
  ASSERT_GE(total_data_read, 0);
  ASSERT_FALSE(all_data.empty());
  std::cout << all_data.data() << std::endl;

  ASSERT_EQ(device.stop(), 0);
}

TEST(asyncreader, read_ton_of_data) {
  // Bash command to output chunks of data
  // while ; do head /dev/urandom | tr -dc A-Za-z0-9 | head -c 256 ; echo '';
  // done
  std::vector<const char *> args{{"./data_out.sh", nullptr}};
  nvimrpc::ReprocDevice device;
  ASSERT_EQ(device.start(args, 10), 0);
  nvimrpc::ReprocAsyncReader reader{device};

	const int TEST_DURATION_SECONDS = 1;
	const size_t POLL_TIMEOUT = 1;
	const size_t EXPECTED_DATA_SIZE = 100;

  auto start = std::chrono::high_resolution_clock::now();

  while (true) {
    auto result = reader.poll(POLL_TIMEOUT);
    ASSERT_TRUE(result);
    auto data = result.value();
		EXPECT_EQ(data.size(), EXPECTED_DATA_SIZE);
    std::cout << data.data() << std::endl;
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration =
        std::chrono::duration_cast<std::chrono::seconds>(stop - start);
		if (duration.count() > TEST_DURATION_SECONDS)
			break;
  }

	device.stop();
}
