/// @file nvim.hpp
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

#include <cstring>
#include <future>
#include <mutex>
#include <reproc++/reproc.hpp>
#include <reproc++/sink.hpp>
#include <string>
#include <string_view>
#include <system_error>
#include <unordered_map>
#include <vector>

#include "easylogging++.h"
#include "mpack.h"

namespace nvim {

class IoDevice {

public:
  IoDevice() = default;
  virtual ~IoDevice() = default;

  virtual size_t send(const char *, size_t) = 0;
  virtual size_t recv(char *, size_t) = 0;
  size_t send(std::string_view s) { return send(s.data(), s.length()); }
};

class ReprocDevice : public IoDevice {
private:
  reproc::process process;
  std::mutex m;
  std::string output;
  std::future<std::error_code> drain_async;

public:
  ReprocDevice() = default;

  /**
   * @brief Spawns and waits for it to start
   * @param argv Command line and arguments to execute
   * @param timeout Number of seconds to wait for process to start
   * Note: Will wait an extra 2 seconds in order to send the terminate and kill
	 * @return 0 in case of success, less than that otherwise
   */
  int spawn(std::string_view argv, int timeout) {
    if (timeout <= 0) {
      LOG(WARNING, "Invalid timeout sent, using 4");
      timeout = 4;
    }

    if (argv.empty()) {
      LOG(ERROR, "Empty argv argument");
      return -1;
    }

    reproc::stop_actions stop_actions{
        {reproc::stop::wait, reproc::milliseconds(timeout * 1000)},
        {reproc::stop::terminate, reproc::milliseconds(1000)},
        {reproc::stop::kill, reproc::milliseconds(1000)},
    };

    reproc::options options;
    options.stop_actions = stop_actions;

    if (const auto ec = process.start(argv.data(), options)) {
      if (ec == std::errc::no_such_file_or_directory)
        throw std::runtime_error(
            "Executable not found, make sure it's in PATH");
      throw std::runtime_error(ec.message());
    }

    drain_async = std::async(std::launch::async, [this]() {
      reproc::sink::thread_safe::string sink(output, output, m);
      return process.drain(sink);
    });

    return 0;
  }

  void kill() {}

  size_t send(const char *buf, size_t size) {
    const auto ec = process.write(reinterpret_cast<const uint8_t *>(buf), size);
    if (ec) {
      throw std::runtime_error(ec.message());
    }
    return size;
  }

  size_t recv(char *buf, size_t size) {
    std::lock_guard<std::mutex> guard(m);
    const auto read_size = std::min<size_t>(size, output.size());
    std::memcpy(buf, output.data(), read_size);
    output.erase(output.begin(), output.begin() + read_size);
    return read_size;
  }
};
} // namespace nvim
