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
#include <chrono>
#include "easylogging++.h"
#include "rpc/iodevice.hpp"
// #include "rpc/streamdecoder.hpp"
// #include "mpack.h"

INITIALIZE_EASYLOGGINGPP

// static void log_server_pack_node(mpack_node_t node) {
	// std::string log_str;
	// auto callback = [](void *context, const char *data, size_t data_len)
	// {
		// auto pstring = (std::string *)(context);
		// pstring->insert(pstring->end(), data, data + data_len);
	// };
	// mpack_node_print_to_callback(node, callback, &log_str);

	// while(true){
		// auto index = log_str.find("\n", 0);
		// if(index == std::string::npos){
			// break;
		// }
		// log_str.replace(index, 1, "\\n");
	// }
	// std::cout << "log_str = '" << log_str << "'" << std::endl;
// }

TEST(nvimrpc, start_stop) {
	nvimrpc::ReprocDevice device;

	std::chrono::seconds timeout{10};
	std::string data;
	// int brecv = 0;
	std::vector<const char *> args{ {"cmake", "--help", nullptr} };

	ASSERT_EQ(device.spawn(args, 10), 0);

	// sleep(1);
	// brecv = device.recv(data, timeout);
	while (device.recv(data, timeout) != 0) {
		std::cout << "data: '" << data << "'" << std::endl;
	}

	device.kill();
}

TEST(nvimrpc, streamdecoder) {
	// int timeout = 10;
	// nvimrpc::ReprocDevice device;
	// std::vector<const char *> args{ {"nvim", "--embed", nullptr} };
	// ASSERT_EQ(device.spawn(args, timeout), 0);

	// nvimrpc::StreamDecoder sd{device};

	// auto node = sd.poll();
	// ASSERT_TRUE(node.has_value());
	// log_server_pack_node(node.value());
}
