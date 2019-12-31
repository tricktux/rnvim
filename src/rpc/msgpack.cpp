/// @file msgpack.cpp
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

#include "rpc/msgpack.hpp"

void nvimrpc::MPackReqPack::mpack_write(
    mpack_writer_t *writer,
    const std::unordered_map<std::string, object_wrapper> &object_map) {
  mpack_start_map(writer, object_map.size());
  for (const auto &val : object_map) {
    mpack_write(writer, val.first);
    mpack_write(writer, val.second);
  }
  mpack_finish_map(writer);
}

void nvimrpc::MPackReqPack::mpack_write(mpack_writer_t *writer,
                                        const object &obj) {
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

/**
 * @brief Finalize the package and return it in binary form
 *
 * @return The data in the string return is packaged and ready to be
 * transmitted.
 */
std::string nvimrpc::MPackReqPack::build() {
  mpack_finish_array(&writer);
  if (mpack_error_t error = mpack_writer_destroy(&writer); error != mpack_ok) {
    DLOG(ERROR) << "Error flushing and closing the underlying stream";
    return {};
  }

  if (data == nullptr) {
    DLOG(ERROR) << "Error invalid pointer to data";
    return {};
  }

  if (size == 0) {
    DLOG(ERROR) << "Error data size is zero";
    return {};
  }

  return {data, size};
}

/**
 * @brief Initializes the stream reader with @p data
 * @param data Data to unpack as a response message
 * @return 0 in case of success, less than zero if error
 */
int nvimrpc::MpackResUnPack::set_data(std::string_view data) {
  if (data.empty()) {
    DLOG(ERROR) << "Empty data provided";
    return -1;
  }

  mpack_reader_init_data(&reader, data.data(), data.size());

  if (mpack_expect_array_max(&reader, NUM_ELEMENTS) != NUM_ELEMENTS) {
    DLOG(ERROR) << "Expected array response of size 4";
    return -2;
  }

  return 0;
}

int nvimrpc::MpackResUnPack::get_error() {
  const mpack_tag_t error = mpack_peek_tag(&reader);
  if (error.type == mpack_type_nil) {
    mpack_discard(&reader); // Disregard the error element
    return 0;
  }

  if (error.type == mpack_type_str) {
    char *buf = mpack_expect_utf8_cstr_alloc(&reader, MAX_CSTR_SIZE);
    DLOG(ERROR) << "Response message contains error message: '" << buf << "'";
    MPACK_FREE(buf);
    return -1;
  }
  if (error.type == mpack_type_int) {
    int rc = mpack_expect_int(&reader);
    DLOG(ERROR) << "Response message contains error code: '" << rc << "'";
    return rc;
  }

  DLOG(ERROR) << "Response message contains error of unknown type";
  mpack_discard(&reader); // Disregard the error element
  return -1;
}

template <typename T> auto nvimrpc::MpackResUnPack::get_result() {
  const mpack_tag_t result = mpack_peek_tag(&reader);

  if constexpr (std::is_void<T>::value) {
    if (result.type != mpack_type_nil)
      DLOG(ERROR) << "Expected nil return type but got: '"
                  << std::to_string(result.type) << "'";
    return;
  }

  T rvalue = mpack_read<T>(&reader);
  mpack_done_array(&reader);

  if (mpack_reader_destroy(&reader) == mpack_ok) {
    DLOG(ERROR) << "Could not unpack response: '" << T{} << "'";
  }
  return std::forward<T>(rvalue);
}

template <typename T>
std::vector<T>
nvimrpc::MpackResUnPack::mpack_read_array(mpack_reader_t *reader) {
  const size_t size = mpack_expect_array(reader);
  std::vector<T> res;
  res.reserve(size);

  for (size_t k = 0; k < size; k++)
    res.emplace_back(mpack_read<T>(reader));

  mpack_done_array(reader);
  return res;
}

nvimrpc::object
nvimrpc::MpackResUnPack::mpack_read_object(mpack_reader_t *reader) {
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
std::unordered_map<std::string, nvimrpc::object>
nvimrpc::MpackResUnPack::mpack_read<
    std::unordered_map<std::string, nvimrpc::object>>(mpack_reader_t *reader) {
  const size_t size = mpack_expect_map(reader);

  std::unordered_map<std::string, object> res;
  res.reserve(size);

  for (size_t i = 0; i < size; ++i) {
    char *buf = mpack_expect_utf8_cstr_alloc(reader, MAX_CSTR_SIZE);
    res[{buf}] = mpack_read_object(reader);
    MPACK_FREE(buf);
  }

  mpack_done_map(reader);
  return res;
}
