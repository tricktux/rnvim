/// @file test_nvimrpc.cpp
/// @brief Tests for the nvimrpc library
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
#include "easylogging++.h"
#include "rpc/nvimrpc.hpp"

INITIALIZE_EASYLOGGINGPP

TEST(nvimrpc, start_stop) {
	nvim::ReprocDevice device;

	int timeout = 10;
	char received[1024];
	int brecv = 0;
	std::vector<const char *> args{ {"nvim", "--embed", nullptr} };

	ASSERT_EQ(device.spawn(args, timeout), 0);

	sleep(1);
	brecv = device.recv(received, 10);
	while (brecv != 0) {
		std::cout << "received: '" << received << "'" << std::endl;
		brecv = device.recv(received, 10);
	}

	device.kill();
}
