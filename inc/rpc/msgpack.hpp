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

#include <utility>
#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>
#include <variant>
#include <vector>

namespace nvimrpc {

// be careful of std::vector<object> vs std::vector<object_wrapper>
// this can easily give bug
class object_wrapper;
using object = std::variant<bool, int64_t, double, std::string,
                            std::vector<object_wrapper>,
                            std::unordered_map<std::string, object_wrapper>>;

class object_wrapper {
  // use shared_ptr not unique_ptr
  // the sematics of object_wrapper is a wrapper of an existing object
  std::shared_ptr<object> m_ptr;

  // don't define operator object ()
  // since which returns a copy of the included object

public:
  object_wrapper() = default;

  template <typename T> object_wrapper(T t) {
    m_ptr = std::make_shared<object>(t);
  }

  object &ref() { return *(m_ptr.get()); }
  const object &ref() const { return *(m_ptr.get()); }
};

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

/** @brief Nicely package a msgpack request
 *
 * @warning Class expects the following calling order:
 *	1. `set_msgid`
 *	2. `set_method`
 *	3. `set_params`
 *	4. `build`
 *
 * */
class MPackReqPack : public IMpackReqPack {
  char *data;
  size_t size;
  mpack_writer_t writer;

  void mpack_write(mpack_writer_t *writer) { mpack_write_nil(writer); }
  void mpack_write(mpack_writer_t *writer, std::string_view value) {
    mpack_write(writer, value.data());
  }
  void mpack_write(mpack_writer_t *writer, std::string &&value) {
    mpack_write(writer, value.data());
  }
  void mpack_write(mpack_writer_t *writer,
                   const std::vector<object_wrapper> &object_list) {
		mpack_start_array(writer, object_list.size());
		for (const auto &val : object_list) {
			mpack_write(writer, val);
		}
		mpack_finish_array(writer);
  }
  void mpack_write(
      mpack_writer_t *writer,
      const std::unordered_map<std::string, object_wrapper> &object_map) {
    mpack_start_map(writer, object_map.size());
    for (const auto &val : object_map) {
      mpack_write(writer, val.first);
      mpack_write(writer, val.second);
    }
    mpack_finish_map(writer);
  }
  void mpack_write(mpack_writer_t *writer, const object &obj) {
    if (std::holds_alternative<bool>(obj))
      return mpack_write(writer, std::get<bool>(obj));
    if (std::holds_alternative<int64_t>(obj))
      return mpack_write(writer, std::get<int64_t>(obj));
    if (std::holds_alternative<double>(obj))
      return mpack_write(writer, std::get<double>(obj));
    if (std::holds_alternative<std::string>(obj))
      return mpack_write(writer, std::get<std::string>(obj));
    if (std::holds_alternative<std::unordered_map<std::string, object_wrapper>>(
            obj)) {
      return mpack_write(
          writer,
          std::get<std::unordered_map<std::string, object_wrapper>>(obj));
    }
    if (std::holds_alternative<std::vector<object_wrapper>>(obj)) {
      return mpack_write(writer, std::get<std::vector<object_wrapper>>(obj));
    }

    DLOG(ERROR) << "Unrecognized object type!";
  }

  template <typename T, typename... Params>
  void mpack_write(mpack_writer_t *writer, T value, Params &&... params) {
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
  /**
   * @brief Finalize the package and return it in binary form
   *
   * @return The data in the string return is packaged and ready to be
   * transmitted.
   */
  std::string build() override;
};

} // namespace nvimrpc
#endif
