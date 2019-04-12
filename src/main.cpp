/// @file main.cpp
/// @brief Main for gnvim
/// @author Reinaldo Molina
/// @version  0.0
/// @date Mar 05 2019
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
// along with this program.  If not, see <http:www.gnu.org/licenses/>.

#include "libnvc.hpp"
#include <cinttypes>
#include <stdexcept>
#include <iostream>
#include <string>

#include <reproc++/reproc.hpp>
#include <gflags/gflags.h>
#include <glog/logging.h>
#include <SFML/Window.hpp>

int main(int argc, char **argv) {
	google::InitGoogleLogging("Cool");

	gflags::ParseCommandLineFlags(&argc, &argv, true);
	LOG(INFO) << "Super cool";

	sf::Window window(sf::VideoMode(800, 600), "My Window");

	std::string casa;
  libnvc::asio_socket socket;
  if (!socket.connect("localhost", 6666)) {
    throw std::runtime_error("failed to connect to localhost:6666");
  }

  LOG(INFO) << 'd';
  if (!argc)
	  LOG(ERROR) << "this is bad";
  libnvc::api_client client(&socket);
  client.forward<libnvc::reqid("nvim_input")>(
      {"$i123<CR>123<ESC>"}, [](int64_t len_done) {
        char buf[128];
        std::sprintf(buf, "nvim_input returns: %" PRIi64, len_done);
        libnvc::log(libnvc::LOG_INFO, buf);
      });

  client.forward<libnvc::reqid("nvim_buf_set_name")>(
      {1, "1234"},
      []() { libnvc::log(libnvc::LOG_INFO, "nvim_buf_set_name done"); },
      [](int64_t ec, std::string emsg) {
        std::printf("nvim reports error: [%d, %s]", (int)(ec), emsg.c_str());
      });

  client.forward<libnvc::reqid("nvim_command")>(
      {":echomsg \"hello world\""},
      []() { libnvc::log(libnvc::LOG_INFO, "nvim_command done"); });

  while (true) {
    client.poll();
  }
}
