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
#include <vector>

/**
 * @brief Spawns and waits for it to start
 * @param argv Command line and arguments to execute
 * @param timeout Number of seconds to wait for process to start
 * Note: Will wait an extra 2 seconds in order to send the terminate and kill
 * @return 0 in case of success, less than that otherwise
 */
int nvimrpc::ReprocDevice::spawn(const std::vector<const char *> &argv,
                                 std::optional<std::chrono::seconds> timeout) {
	std::chrono::seconds s = timeout.value_or(4);

  if (argv.empty()) {
    DLOG(ERROR) << "Empty argv argument";
    return -1;
  }

  reproc::stop_actions stop_actions{
      {reproc::stop::wait, std::chrono::seconds{s}},
      {reproc::stop::terminate, std::chrono::seconds{1}},
      {reproc::stop::kill, std::chrono::seconds{1}},
  };

  reproc::options options;
  options.stop_actions = stop_actions;

  if (auto ec = process.start(argv.data(), options)) {
    DLOG(ERROR) << "Error occurred trying to spawn: '" << argv.data()
                << "'. Error message: '" << ec.message() << "'";
    if (ec == std::errc::no_such_file_or_directory)
      throw std::runtime_error("Executable not found, make sure it's in PATH");
    throw std::runtime_error(ec.message());
  }

  drain_async = std::async(std::launch::async, [this]() {
    reproc::sink::thread_safe::string sink(output, output, m);
    return process.drain(sink);
  });

  return 0;
}

/** @brief Terminates the child process */
void nvimrpc::ReprocDevice::kill() {
  if (!process.running()) {
    DLOG(WARNING) << "Child process already dead";
    return;
  }

  reproc::stop_actions stop_actions{
      {reproc::stop::wait, std::chrono::seconds{1}},
      {reproc::stop::terminate, std::chrono::seconds{1}},
      {reproc::stop::kill, std::chrono::seconds{1}},
  };

  if (auto ec = process.stop(stop_actions)) {
    DLOG(ERROR) << "Error: '" << ec.message()
                << "' occurred while killing child process";
    return;
  }

  DLOG(INFO) << "Gracefully closed child process whit exit code: "
             << process.exit_status();
}

/**
 * @brief Sends `buf` through `stdin` to the child process
 * @param buf Pointer to bytes to send
 * @param size Number of bytes
 * @return Number of bytes sent
 */
size_t nvimrpc::ReprocDevice::send(std::string_view data) {
  if (!process.running()) {
    DLOG(FATAL) << "Child process is not running!";
    throw std::runtime_error("Child process is not running!");
  }

  if (data.empty()) {
    DLOG(WARNING) << "Empty send data provided";
    return 0;
  }

  if (auto ec = process.write(reinterpret_cast<const uint8_t *>(data.data()),
                              data.size())) {
    DLOG(ERROR) << "Failed to send: '" << data << "'. Error message: '"
                << ec.message() << "'";
    throw std::runtime_error(ec.message());
  }
  return data.size();
}

/**
 * @brief Receive data from buffer, wait for @seconds
 * @param buf Pointer to storage for received data
 * @param size Size of storage
 * @return Number of bytes copied to buffer
 */
size_t
nvimrpc::ReprocDevice::recv(std::string &data,
                            std::optional<std::chrono::seconds> timeout) {
  if (!process.running()) {
    DLOG(FATAL) << "Child process is not running!";
    throw std::runtime_error("Child process is not running!");
  }

  if (timeout) {
    std::future_status fs = drain_async.wait_for(timeout.value());
    if (fs == std::future_status::timeout) {
      DLOG(INFO) << "Timed out waiting for data";
      return 0;
    }
  }
  std::lock_guard<std::mutex> guard(m);
  if (output.empty()) {
    DLOG(INFO) << "No output data";
    return 0;
  }

  data = output;
  output.clear();
  return data.size();
}
