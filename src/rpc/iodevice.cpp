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

/**
 * @brief Spawns and waits for it to start
 * @param argv Command line and arguments to execute
 * @param timeout Number of seconds to wait for process to start
 * Note: Will wait an extra 2 seconds in order to send the terminate and kill
 * @return 0 in case of success, less than that otherwise
 */
int nvim::ReprocDevice::spawn(const std::vector<const char *> &argv,
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
void nvim::ReprocDevice::kill() {
  if (!process.running()) {
    DLOG(WARNING) << "Child process already dead";
    return;
  }

  reproc::stop_actions stop_actions{
      {reproc::stop::wait, reproc::milliseconds(1000)},
      {reproc::stop::terminate, reproc::milliseconds(1000)},
      {reproc::stop::kill, reproc::milliseconds(1000)},
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
size_t nvim::ReprocDevice::send(const char *buf, size_t size) {
  if (!process.running()) {
    DLOG(FATAL) << "Child process is not running!";
    throw std::runtime_error("Child process is not running!");
  }

  if ((buf == nullptr) || (buf[0] == 0)) {
    DLOG(WARNING) << "Invalid buf pointer";
    return 0;
  }

  if (size == 0) {
    DLOG(WARNING) << "Size zero provided";
    return 0;
  }

  if (auto ec = process.write(reinterpret_cast<const uint8_t *>(buf), size)) {
    DLOG(ERROR) << "Failed to send: '" << buf << "'. Error message: '"
                << ec.message() << "'";
    throw std::runtime_error(ec.message());
  }
  return size;
}

/**
 * @brief Receive
 * @param buf
 * @param size
 * @return
 */
size_t nvim::ReprocDevice::recv(char *buf, size_t size) {
  if (!process.running()) {
    DLOG(FATAL) << "Child process is not running!";
    throw std::runtime_error("Child process is not running!");
  }

  if (buf == nullptr) {
    DLOG(WARNING) << "Invalid buf pointer";
    return 0;
  }

  if (size == 0) {
    DLOG(WARNING) << "Size zero provided";
    return 0;
  }

  std::lock_guard<std::mutex> guard(m);
  size_t read_size = std::min<size_t>(size, output.size());
  std::memcpy(buf, output.data(), read_size);
  output.erase(output.begin(), output.begin() + read_size);
  return read_size;
}
