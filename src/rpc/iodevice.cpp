/// @file iodevice.cpp
/// @brief Code for a neovim rpc client
/// @author Reinaldo Molina
/// @version  0.0
/// @date Dec 17 2019
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
#include <algorithm>
#include <chrono>
#include <vector>

/**
 * @brief Spawns and waits for it to start
 * @param argv Command line and arguments to execute
 * @param timeout Number of seconds to wait for process to start
 * Note: Will wait an extra 2 seconds in order to send the terminate and kill
 * @return 0 in case of success, less than that otherwise
 */
int nvimrpc::ReprocDevice::start(const std::vector<const char *> &argv,
                                 int timeout) {
  if (timeout <= 0) {
    DLOG(WARNING) << "Invalid timeout sent, using 4 seconds";
    timeout = 4;
  }

  if (argv.empty()) {
    DLOG(ERROR) << "Empty argv argument";
    return -1;
  }

  reproc::stop_actions stop_actions{
      {reproc::stop::wait, reproc::milliseconds(timeout * 1000)},
      {reproc::stop::terminate, reproc::milliseconds(1000)},
      {reproc::stop::kill, reproc::milliseconds(1000)},
  };

  reproc::options options;
  options.stop = stop_actions;

  if (auto ec = process.start(argv.data(), options)) {
    DLOG(ERROR) << "Error occurred trying to start: '" << argv.data()
                << "'. Error message: '" << ec.message() << "'";
    if (ec == std::errc::no_such_file_or_directory)
      throw std::runtime_error("Executable not found, make sure it's in PATH");
    throw std::runtime_error(ec.message());
  }

  return 0;
}

/** @brief Terminates the child process */
int nvimrpc::ReprocDevice::stop() {
  reproc::stop_actions stop_actions{
      {reproc::stop::wait, std::chrono::seconds{1}},
      {reproc::stop::terminate, std::chrono::seconds{1}},
      {reproc::stop::kill, std::chrono::seconds{1}},
  };

  auto [exit_status, ec] = process.stop(stop_actions);
  if (ec) {
    DLOG(ERROR) << "Error: '" << ec.message()
                << "' occurred while killing child process";
    return ec.value();
  }

  DLOG(INFO) << "Gracefully closed child process whit exit code: "
             << exit_status;
  return exit_status;
}

/**
 * @brief Sends `buf` through `stdin` to the child process
 * @param buf Pointer to bytes to send
 * @param size Number of bytes
 * @return Number of bytes sent
 */
size_t nvimrpc::ReprocDevice::write(std::string_view data) {
  if (data.empty()) {
    DLOG(WARNING) << "Empty send data provided";
    return 0;
  }

  if (auto ec = process.write(reinterpret_cast<const uint8_t *>(data.data()),
                              data.size())) {
    DLOG(FATAL) << "Failed to send: '" << data << "'. Error message: '"
                << ec.message() << "'";
    throw std::runtime_error(ec.message());
  }
  return data.size();
}

/**
 * @brief Receive data from buffer, wait for @p @timeout for data to arrive
 * @param data Data storage
 * @param timeout Number of seconds to wait for data to arrive
 * @return Size of rec'd data
 */
size_t nvimrpc::ReprocDevice::read(uint8_t *buf, size_t size) {
  if (buf == nullptr) {
    DLOG(WARNING) << "Invalid buf pointer";
    return 0;
  }

  if (size == 0) {
    DLOG(WARNING) << "Size zero provided";
    return 0;
  }

	// std::tuple<reproc::stream, sizet, std::error_code>
	auto [rstream, bytes_read, ec] = process.read(buf, size);
	if (ec) {
		DLOG(FATAL) << "Error reading from process: '" << ec.message() << "'";
	}

	if (rstream == reproc::stream::err) {
		DLOG(WARNING) << "Bytes read come from stderr";
	}
  return bytes_read;
}

void nvimrpc::ReprocAsyncReader::wait_for_data() {
	size_t data_read;
	std::array<uint8_t, ARRAY_SIZE> data{};
	uint8_t *ptr_data = data.data();
	while (true) {
		if ((data_read = dev.read(ptr_data, ARRAY_SIZE)) > 0) {
			push(ptr_data, data_read);
		}
	}
}
