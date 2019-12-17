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

  void spawn(std::string_view argv) {
    reproc::stop_actions stop_actions{
        {reproc::stop::terminate, reproc::milliseconds(5000)},
        {reproc::stop::kill, reproc::milliseconds(2000)},
        {},
    };

    reproc::options options;
    options.stop_actions = stop_actions;

    const auto ec =
        process.start(argv.empty() ? "nvim --embed" : argv.data(), options);
    if (ec == std::errc::no_such_file_or_directory) {
      throw std::runtime_error("neovim not found, make sure it's in PATH");
    }

    if (ec) {
      throw std::runtime_error(ec.message());
    }

    drain_async = std::async(std::launch::async, [this]() {
      reproc::sink::thread_safe::string sink(output, output, m);
      return process.drain(sink);
    });
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
