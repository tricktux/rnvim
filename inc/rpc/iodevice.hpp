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
#include <atomic>
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
  IoDevice(const IoDevice &dev) = delete;
  IoDevice(IoDevice &&dev) = delete;
  void operator=(const IoDevice &dev) = delete;
  void operator=(IoDevice &&dev) = delete;

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
  int start(const std::vector<const char *> &argv, int timeout) override;
  int stop() override;
  size_t write(std::string_view data) override;
  size_t read(uint8_t *buf, size_t size) override;
};

class IIoAsyncReader {
protected:
  std::atomic<bool> done;
  std::mutex qm;
  std::condition_variable cv;
  std::vector<uint8_t> data;
  std::thread t;

  const static size_t ARRAY_SIZE = 4096;
  const static size_t DATA_SIZE = ARRAY_SIZE * 20;

  virtual void wait_for_data() = 0; /// Waits for IoDevice::read to return data
  void push(uint8_t *buf, size_t size) {
    if (buf == nullptr) {
      return;
    }
    std::unique_lock<std::mutex> lk(qm);
    data.insert(std::end(data), buf, buf + size);
    cv.notify_one();
  }
  void stop() {
    if (t.joinable()) {
      done.store(true);
      t.join();
    }
  }
  IIoAsyncReader() : done(false), t(&IIoAsyncReader::wait_for_data, this) {
    data.reserve(DATA_SIZE);
  }
  virtual ~IIoAsyncReader() { stop(); }

public:
  IIoAsyncReader(IIoAsyncReader &&reader) = delete;
  IIoAsyncReader(const IIoAsyncReader &reader) = delete;
  IIoAsyncReader &operator=(IIoAsyncReader &&reader) = delete;
  IIoAsyncReader &operator=(const IIoAsyncReader &reader) = delete;
  /**
   * @brief Poll reader for data
   * @param timeout Wait only for @p timeout seconds
   * @return data if there is any, empty if timed out
   */
  std::optional<std::vector<uint8_t>> poll(size_t timeout) {
    std::unique_lock<std::mutex> lk(qm);
    if (!cv.wait_for(lk, std::chrono::seconds{timeout},
                     [this] { return !data.empty(); })) {
      return {};
    };
    // std::swap(data, buffer);
    std::vector<uint8_t> buffer{data};
    data.clear();
    return buffer;
  }
};

class ReprocAsyncReader : public IIoAsyncReader {
  nvimrpc::IoDevice &dev; // Used to read data
  void wait_for_data() override;

public:
  ReprocAsyncReader(ReprocAsyncReader &&other) = delete;
  ReprocAsyncReader(const ReprocAsyncReader &other) = delete;
  ReprocAsyncReader &operator=(ReprocAsyncReader &&other) = delete;
  ReprocAsyncReader &operator=(const ReprocAsyncReader &other) = delete;
  explicit ReprocAsyncReader(nvimrpc::IoDevice &_dev) : dev(_dev) {}
  ~ReprocAsyncReader() override {
    stop();
    dev.stop();
  }
};

} // namespace nvimrpc

#endif
