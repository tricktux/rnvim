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
typedef std::variant<bool, int64_t, double, std::string,
                     std::vector<object_wrapper>,
                     std::unordered_map<std::string, object_wrapper>>
    object;

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

void inline mpack_write(mpack_writer_t *) {}

template <typename T, typename... Params>
void inline mpack_write(mpack_writer_t *writer, T&& value, Params &&... params);

class IMpackReqPack {
public:
  const static size_t NUM_ELEMENTS = 4;
  const static size_t MSG_TYPE = 0;

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
public:
  const static size_t NUM_ELEMENTS = 4;
  const static size_t MSG_TYPE = 1;

  IMpackResUnPack() = default;
  virtual ~IMpackResUnPack() = default;

  virtual int set_data(std::string_view) = 0;
  virtual int get_msg_type() = 0;
  virtual size_t get_msgid() = 0;
  virtual int get_error() = 0;
  // This function cannot be virtual because it uses templates
  // auto get_result();
};

template <typename T> T mpack_read(mpack_reader_t *);
class MpackResUnPack : public IMpackResUnPack {
	void close_mpack() {
		mpack_done_array(&reader);

		if (mpack_reader_destroy(&reader) != mpack_ok) {
			DLOG(ERROR) << "Could not unpack response";
		}
	}

public:
  const static size_t MAX_CSTR_SIZE = 1048576;
  mpack_reader_t reader;

  MpackResUnPack() = default;
  virtual ~MpackResUnPack() = default;

  int set_data(std::string_view data) override;
  int get_msg_type() override { return mpack_expect_i32(&reader); }
  size_t get_msgid() override { return mpack_expect_u32(&reader); }
  int get_error() override;

  // This function cannot be virtual because it uses templates
  template <typename T> T get_result() {
    const mpack_tag_t result = mpack_peek_tag(&reader);

    if constexpr (std::is_void<T>::value) {
      if (result.type != mpack_type_nil)
        DLOG(ERROR) << "Expected nil return type but got: '"
                    << std::to_string(result.type) << "'";
      return;
    }

    T rvalue = mpack_read<T>(&reader);
    mpack_done_array(&reader);

    if (mpack_reader_destroy(&reader) != mpack_ok) {
      DLOG(ERROR) << "Could not unpack response";
    }
    return std::forward<T>(rvalue);
  }
};

template <> void MpackResUnPack::get_result<void>() {
	const mpack_tag_t result = mpack_peek_tag(&reader);

	if (result.type != mpack_type_nil)
		DLOG(ERROR) << "Expected nil return type but got: '"
			<< std::to_string(result.type) << "'";

	mpack_discard(&reader);
	mpack_done_array(&reader);

	if (mpack_reader_destroy(&reader) != mpack_ok) {
		DLOG(ERROR) << "Could not unpack response";
	}
	return;
}

// --------------mpack_write--------------------- //

void inline mpack_write(mpack_writer_t *writer, const object &obj) {
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
        writer, std::get<std::unordered_map<std::string, object_wrapper>>(obj));
  }
  if (std::holds_alternative<std::vector<object_wrapper>>(obj)) {
    return mpack_write(writer, std::get<std::vector<object_wrapper>>(obj));
  }

  DLOG(ERROR) << "Unrecognized object type!";
}

void inline mpack_write(mpack_writer_t *writer, std::string_view value) {
  mpack_write_utf8_cstr(writer, value.data());
}
void inline mpack_write(mpack_writer_t *writer, const std::string &value) {
  mpack_write_utf8_cstr(writer, value.data());
}
inline void
mpack_write(mpack_writer_t *writer,
            const std::unordered_map<std::string, object_wrapper> &object_map);

void inline mpack_write(
    mpack_writer_t *writer,
    const std::unordered_map<std::string, object_wrapper> &object_map) {
  mpack_start_map(writer, object_map.size());
  for (const auto &val : object_map) {
    mpack_write(writer, val.first);
    mpack_write(writer, val.second);
  }
  mpack_finish_map(writer);
}
void inline mpack_write(mpack_writer_t *writer,
                        const std::vector<object_wrapper> &object_list) {
  mpack_start_array(writer, object_list.size());
  for (const auto &val : object_list) {
    mpack_write(writer, val);
  }
  mpack_finish_array(writer);
}

template <typename T, typename... Params>
void inline mpack_write(mpack_writer_t *writer, T&& value, Params &&... params) {
	mpack_write(writer, std::forward<T>(value));
	mpack_write(writer, std::forward<Params>(params)...);
}

// void mpack_write(mpack_writer_t *writer, const std::array<int64_t, 2> &val) {
// mpack_write_i64(writer, val[0]);
// mpack_write_i64(writer, val[1]);
// }

// --------------mpack_read--------------------- //
template <typename T> T mpack_read(mpack_reader_t *);
template <> bool inline mpack_read<bool>(mpack_reader_t *reader) {
  return mpack_expect_bool(reader);
}
template <> int64_t inline mpack_read<int64_t>(mpack_reader_t *reader) {
  return mpack_expect_i64(reader);
}
template <> uint64_t inline mpack_read<uint64_t>(mpack_reader_t *reader) {
  return mpack_expect_u64(reader);
}
template <> double inline mpack_read<double>(mpack_reader_t *reader) {
  return mpack_expect_double(reader);
}
template <> std::string inline mpack_read<std::string>(mpack_reader_t *reader) {
  char *buf =
      mpack_expect_utf8_cstr_alloc(reader, MpackResUnPack::MAX_CSTR_SIZE);
  std::string res{buf};
  MPACK_FREE(buf);
  return res;
}
template <typename T>
std::vector<T> inline mpack_read_array(mpack_reader_t *reader);

template <>
std::vector<int64_t> inline mpack_read<std::vector<int64_t>>(
    mpack_reader_t *reader) {
  return mpack_read_array<int64_t>(reader);
}
template <>
std::vector<bool> inline mpack_read<std::vector<bool>>(mpack_reader_t *reader) {
  return mpack_read_array<bool>(reader);
}
template <>
std::vector<uint64_t> inline mpack_read<std::vector<uint64_t>>(
    mpack_reader_t *reader) {
  return mpack_read_array<uint64_t>(reader);
}
template <>
std::vector<double> inline mpack_read<std::vector<double>>(
    mpack_reader_t *reader) {
  return mpack_read_array<double>(reader);
}
template <>
std::vector<std::string> inline mpack_read<std::vector<std::string>>(
    mpack_reader_t *reader) {
  return mpack_read_array<std::string>(reader);
}

inline object mpack_read_object(mpack_reader_t *reader);

template <>
std::vector<std::unordered_map<std::string, object>> inline mpack_read<
    std::vector<std::unordered_map<std::string, object>>>(
    mpack_reader_t *reader) {
  return mpack_read_array<std::unordered_map<std::string, object>>(reader);
}

template <>
inline std::unordered_map<std::string, object>
mpack_read<std::unordered_map<std::string, object>>(mpack_reader_t *reader);

template <typename T>
std::vector<T> inline mpack_read_array(mpack_reader_t *reader) {
  const size_t size = mpack_expect_array(reader);
  std::vector<T> res;
  res.reserve(size);

  for (size_t k = 0; k < size; k++)
    res.emplace_back(mpack_read<T>(reader));

  mpack_done_array(reader);
  return res;
}

nvimrpc::object inline mpack_read_object(mpack_reader_t *reader) {
  object res{};

  const mpack_tag_t tag = mpack_peek_tag(reader);
  switch (tag.type) {
  case mpack_type_bool:
    return mpack_read<bool>(reader);
  case mpack_type_int:
    return mpack_read<int64_t>(reader);
  case mpack_type_uint:
    return (int64_t)(mpack_read<uint64_t>(reader));
  case mpack_type_double:
    return mpack_read<double>(reader);
  case mpack_type_str:
    return mpack_read<std::string>(reader);
  case mpack_type_map: {
    auto res_map = mpack_read<std::unordered_map<std::string, object>>(reader);
    std::unordered_map<std::string, object_wrapper> res_map_wrapper;
    for (auto &item : res_map)
      res_map_wrapper[item.first] = object_wrapper(item.second);
    return object(res_map_wrapper);
  }
  case mpack_type_array: {
    auto res_vec = mpack_read<std::vector<object>>(reader);
    std::vector<object_wrapper> res_vec_wrapper;
    for (auto &item : res_vec)
      res_vec_wrapper.emplace_back(object_wrapper(item));
    return object(res_vec_wrapper);
  }
  default:
    DLOG(ERROR) << "Object is not of a recognized type";
    return res;
  }
}

template <>
std::unordered_map<std::string, nvimrpc::object> inline mpack_read<
    std::unordered_map<std::string, nvimrpc::object>>(mpack_reader_t *reader) {
  const size_t size = mpack_expect_map(reader);

  std::unordered_map<std::string, object> res;
  res.reserve(size);

  for (size_t i = 0; i < size; ++i) {
    char *buf =
        mpack_expect_utf8_cstr_alloc(reader, MpackResUnPack::MAX_CSTR_SIZE);
    res[{buf}] = mpack_read_object(reader);
    MPACK_FREE(buf);
  }

  mpack_done_map(reader);
  return res;
}

// template <>
// std::array<int64_t, 2>
// mpack_read<std::array<int64_t, 2>>(mpack_reader_t *reader) {
// const int64_t x = mpack_expect_i64(reader);
// const int64_t y = mpack_expect_i64(reader);
// return {x, y};
// }

// --------------mpack_read--------------------- //

} // namespace nvimrpc
#endif
