/// @file iodevice.hpp
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

#ifndef IODEVICE_HPP
#define IODEVICE_HPP

#include <chrono>
#include <cstring>
#include <future>
#include <mutex>
#include <optional>
#include <condition_variable>
#include <reproc++/reproc.hpp>
#include <reproc++/sink.hpp>
#include <string>
#include <string_view>
#include <system_error>
#include <vector>

namespace nvimrpc {

/** @brief Input Output device abstraction used to communicate with Neovim */
class IoDevice {

public:
  IoDevice() = default;
  virtual ~IoDevice() = default;

  virtual size_t send(std::string_view data) = 0;
  virtual size_t recv(std::string &data, size_t timeout) = 0;
	virtual size_t recv(char *, size_t) = 0;
};

/** @brief Device that communicates over `stdin/stdout/stderr`
 *  Chosen method of communicate that is more secure and faster than sockets
 * */
class ReprocDevice : public IoDevice {
private:
  reproc::process process;
  std::mutex m;       /// `Mutex` used by the drain sink to protect output
  std::string output; /// Storage for child process `stdout` and `stderr`
                      /// Future to read async from `stdout` and `stderr` into
                      /// output using the `recv` function
  std::future<std::error_code> drain_async;
	std::condition_variable cv;

public:
  ReprocDevice() { output.reserve(33554432); }
  virtual ~ReprocDevice() {}

	std::error_code drain() {
		reproc::sink::thread_safe::string sink{output, m};
		return reproc::drain(this->process, sink, sink);
	}
  int spawn(const std::vector<const char *> &, int);
  int kill();
  size_t send(std::string_view data) override;
  size_t recv(std::string &data, size_t timeout) override;
	size_t recv(char *, size_t) override;
};

} // namespace nvimrpc

#endif
