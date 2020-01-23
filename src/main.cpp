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

// #include "application.hpp"
// #include "libnvc.hpp"
#include "easylogging++.h"
#include "rpc/iodevice.hpp"
#include "rpc/msgpack.hpp"
#include "nvimapi.hpp"

#include <iostream>
#include <stdexcept>
#include <string>
#include <filesystem>

INITIALIZE_EASYLOGGINGPP

namespace fs = std::filesystem;

void setup_logs() {
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
  std::cout << elconf << std::endl;
  el::Configurations conf(elconf.c_str());
  // Actually reconfigure all loggers instead
  el::Loggers::reconfigureAllLoggers(conf);
  DLOG(INFO) << ">>>>Start of Main Log<<<<";
}

int main(int argc, char **argv) {
  START_EASYLOGGINGPP(argc, argv);

	setup_logs();

  int timeout = 10;
  std::string buf{"yixx"};
  nvimrpc::ReprocDevice device;
  std::vector<const char *> args{{"nvim", "-u", "NORC", "--embed", nullptr}};
  device.spawn(args, timeout);

  nvimrpc::NvimApi api{device};
  api.nvim_ui_attach(3000, 2000, {{"rgb", true}});
  api.nvim_buf_set_name(1, buf);
  buf = api.nvim_buf_get_name(1);
	std::cout << "buf = " << buf << std::endl;
  device.kill();
}
