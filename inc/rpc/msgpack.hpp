/// @file msgpack.hpp
/// @brief Wrapper around MPack
/// @author Reinaldo Molina
/// @version  0.0
/// @date Dec 30 2019
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

#ifndef MSGPACK_HPP
#define MSGPACK_HPP

#include "easylogging++.h"
#include "mpack.h"

#include <string>
#include <string_view>

namespace nvimrpc {

class IMpackReqPack {

public:
  IMpackReqPack() = default;
  virtual ~IMpackReqPack() = default;

  virtual void set_msgid(size_t) = 0;
  virtual void set_method(std::string_view) = 0;
  // template<class...Params>
  // void set_params(Params&&... params);
  virtual std::string build() = 0;
};

class MPackReqPack : public IMpackReqPack {
  char *data;
  size_t size;
  mpack_writer_t writer;

  void mpack_write(mpack_writer_t *) {}

  template <typename T, typename... Params>
  void mpack_write(mpack_writer_t *writer, T value, Params&&... params) {
    mpack_write(writer, value);
    mpack_write(writer, std::forward<Params>(params)...);
  }

public:
  MPackReqPack() : data(nullptr), size(0) {
    mpack_writer_init_growable(&writer, &data, &size);
    // The request message is a four elements array
    mpack_start_array(&writer, 4);
    // The message type, must be the integer zero (0) for "Request" messages.
    mpack_write_i32(&writer, 0);
  }
  virtual ~MPackReqPack() {
    if (data != nullptr) {
      MPACK_FREE(data);
      data = nullptr;
    }
  }

  /**
   * @brief Set rpc request msgid
   * This number is used as a sequence number. The server's response to the
   * "Request" will have the same msgid.
   * @param msgid A 32-bit unsigned integer number.
   */
  void set_msgid(size_t msgid) override { mpack_write_i32(&writer, msgid); }

  /**
   * @brief Set the method name
   * @param name A string which represents the method name.
   */
  void set_method(std::string_view name) override {
    if (name.empty()) {
      DLOG(ERROR) << "Empty method name";
      return;
    }
    mpack_write_utf8_cstr(&writer, name.data());
  }
  template <class... Params> void set_args(Params &&... params);
  std::string build() override;
};

} // namespace nvimrpc
#endif
