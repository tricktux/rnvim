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

  virtual int start(const std::vector<const char *> &, int) = 0;
  virtual int stop() = 0;
  virtual size_t write(std::string_view data) = 0;
  virtual size_t read(uint8_t *, size_t) = 0;
};

/** @brief Device that communicates over `stdin/stdout/stderr`
 *  Chosen method of communicate that is more secure and faster than sockets
 * */
class ReprocDevice : public IoDevice {
private:
  reproc::process process;

public:
  ReprocDevice() = default;
  virtual ~ReprocDevice() = default;

  int start(const std::vector<const char *> &, int) override;
  int stop() override;
  size_t write(std::string_view data) override;
  size_t read(uint8_t *, size_t) override;
};

} // namespace nvimrpc

class IIoAsyncReader {
protected:
  std::mutex qm;
  std::condition_variable cv;
  std::vector<uint8_t> data;
  std::thread t;
  nvimrpc::IoDevice &dev; // Used to read data

  const static size_t ARRAY_SIZE = 409600 ;

  virtual void wait_for_data() = 0; /// Waits for IoDevice::read to return data
  void push(const std::array<uint8_t, ARRAY_SIZE> &_data) {
    if (_data.empty())
      return;
    std::unique_lock<std::mutex> lk(qm);
    data.insert(std::end(data), std::begin(_data), std::end(_data));
    cv.notify_one();
  }
  IIoAsyncReader(nvimrpc::IoDevice &_dev)
      : t(&IIoAsyncReader::wait_for_data, this), dev(_dev) {}
  virtual ~IIoAsyncReader() = default;

public:
  // std::queue<T> poll() {
  std::optional<std::vector<uint8_t>> poll() {
    std::vector<uint8_t> buffer;
    std::unique_lock<std::mutex> lk(qm);
    cv.wait(lk, [this] { return !data.empty(); });
    std::swap(data, buffer);
    return buffer;
  }
};
#endif
