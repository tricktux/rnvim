/// @file msgpack.hpp
//
//
//
//
//
//
//
//
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

#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>
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
  // Use shared_ptr not unique_ptr
  // The sematics of object_wrapper is a wrapper of an existing object
  std::shared_ptr<object> m_ptr;

  // Don't define operator object ()
  // Since which returns a copy of the included object

public:
  object_wrapper() = default;

  template <typename T> object_wrapper(T t) {
    m_ptr = std::make_shared<object>(t);
  }

  object &ref() { return *(m_ptr.get()); }
  const object &ref() const { return *(m_ptr.get()); }
};

class IMpackReqPack {
protected:
  const size_t NUM_ELEMENTS = 4;
  const size_t MSG_TYPE = 0;

public:
  IMpackReqPack() = default;
  virtual ~IMpackReqPack() = default;

  virtual void set_msgid(size_t) = 0;
  virtual void set_method(std::string_view) = 0;
  // Cannot be virtual since it uses templates
  // template<class...Params> void set_params(Params&&... params);
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
    mpack_write_utf8_cstr(writer, value.data());
  }
  void mpack_write(mpack_writer_t *writer, std::string &&value) {
    mpack_write_utf8_cstr(writer, value.data());
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
  void mpack_write(mpack_writer_t *writer, const object &obj);

  template <typename T, typename... Params>
  void mpack_write(mpack_writer_t *writer, T value, Params &&... params) {
    mpack_write(writer, value);
    mpack_write(writer, std::forward<Params>(params)...);
  }

public:
  MPackReqPack() : data(nullptr), size(0) {
    mpack_writer_init_growable(&writer, &data, &size);
    // The request message is a four elements array
    mpack_start_array(&writer, NUM_ELEMENTS);
    // The message type, must be the integer zero (0) for "Request" messages.
    mpack_write_i32(&writer, MSG_TYPE);
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

  /**
   * @brief Pass the method parameters
   * @param params
   */
  template <class... Params> void set_params(Params &&... params) {
    mpack_start_array(&writer, sizeof...(Params));
    mpack_write(&writer, std::forward<Params>(params)...);
    mpack_finish_array(&writer);
  }
  std::string build() override;
};

class IMpackResUnPack {
protected:
  const size_t NUM_ELEMENTS = 4;
  const size_t MSG_TYPE = 1;

public:
  IMpackResUnPack() = default;
  virtual ~IMpackResUnPack() = default;

  virtual int set_data(std::string_view) = 0;
  virtual int get_msg_type() = 0;
  virtual size_t get_msgid() = 0;
  virtual int get_error() = 0;
  // This function cannot be virtual because it uses templates
  // auto get_result();
};

class MpackResUnPack : public IMpackResUnPack {
  mpack_reader_t reader;

public:
  MpackResUnPack() = default;
  virtual ~MpackResUnPack() = default;

  int set_data(std::string_view data) override;
  int get_msg_type() override { return mpack_expect_i32(&reader); }
  size_t get_msgid() override { return mpack_expect_u32(&reader); }
  int get_error() override;
};

} // namespace nvimrpc
#endif
