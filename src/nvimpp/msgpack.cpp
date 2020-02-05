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

#include "nvimpp/msgpack.hpp"

#include "easylogging++.h"
#include <tuple>

/**
 * @brief Finalize the package and return it in binary form
 *
 * @return The data in the string return is packaged and ready to be
 * transmitted.
 */
std::string nvimrpc::MpackRpcPack::build() {
  mpack_finish_array(&writer);
  if (mpack_error_t error = mpack_writer_destroy(&writer); error != mpack_ok) {
    LOG(ERROR) << "Error flushing and closing the underlying stream";
    LOG(ERROR) << "Error: '" << mpack_error_to_string(error) << "'";
    return {};
  }

  if (data == nullptr) {
    LOG(ERROR) << "Error invalid pointer to data";
    return {};
  }

  if (size == 0) {
    LOG(ERROR) << "Error data size is zero";
    return {};
  }

  { // Log written data
    mpack_tree_t tree;
    mpack_tree_init_data(&tree, data, size);
    mpack_tree_parse(&tree);

    mpack_node_t root = mpack_tree_root(&tree);
    log_server_pack_node(root);
    mpack_tree_destroy(&tree);
  }
  return {data, size};
}

std::optional<std::tuple<int64_t, std::string>>
nvimrpc::MpackRpcUnpack::get_error() {
  if (mpack_node_is_nil(node)) {
    LOG(ERROR) << "Empty main node";
    return {};
  }
  if (mpack_node_array_length(node) <= RESPONSE_ERROR_IDX) {
    LOG(ERROR) << "Array in node is smaller than expected";
    return {};
  }

  mpack_node_t error = mpack_node_array_at(node, RESPONSE_ERROR_IDX);
  if (mpack_node_is_nil(error)) // No error to report
    return {};

  if (size_t errnode_size = mpack_node_array_length(error); errnode_size != 2) {
    LOG(ERROR) << ": RESP error array [type, message] has invalid size: '"
               << errnode_size << "'";
    return {};
  }
  int64_t ec = mpack_node_i64(mpack_node_array_at(error, 0));
  const char *msg_ptr = mpack_node_str(mpack_node_array_at(error, 1));
  size_t msg_len = mpack_node_strlen(mpack_node_array_at(error, 1));

  return std::make_tuple(ec, std::string{msg_ptr, msg_len});
}

void nvimrpc::log_server_pack_node(mpack_node_t node) {
  std::string log_str;
  auto callback = [](void *context, const char *data, size_t data_len) {
    auto pstring = (std::string *)(context);
    pstring->insert(pstring->end(), data, data + data_len);
  };
  mpack_node_print_to_callback(node, callback, &log_str);

  while (true) {
    auto index = log_str.find("\n", 0);
    if (index == std::string::npos) {
      break;
    }
    log_str.replace(index, 1, "\\n");
  }
  LOG(INFO) << log_str;
}

inline size_t nvimrpc::MpackRpcUnpack::get_num_elements() {
  if (mpack_node_is_nil(node)) {
    LOG(ERROR) << "Empty node";
    return 0;
  }
  return mpack_node_array_length(node);
}

inline int nvimrpc::MpackRpcUnpack::get_msg_type() {
  if (mpack_node_is_nil(node)) {
    LOG(ERROR) << "Empty node";
    return -1;
  }
  if (mpack_node_array_length(node) <= RESPONSE_MSG_TYPE_IDX) {
    LOG(ERROR) << "Array in node is smaller than expected";
    return -2;
  }

  return mpack_node_u32(mpack_node_array_at(node, RESPONSE_MSG_TYPE_IDX));
}

inline size_t nvimrpc::MpackRpcUnpack::get_msgid() {
  if (mpack_node_is_nil(node)) {
    LOG(ERROR) << "Empty node";
    return 0;
  }
  if (mpack_node_array_length(node) <= RESPONSE_MSG_ID_IDX) {
    LOG(ERROR) << "Array in node is smaller than expected";
    return 0;
  }

  return mpack_node_u32(mpack_node_array_at(node, RESPONSE_MSG_ID_IDX));
}

template <typename T> T nvimrpc::MpackRpcUnpack::get_result() {
  if (mpack_node_is_nil(node)) {
    LOG(ERROR) << "Empty node";
    return T();
  }

  if (mpack_node_array_length(node) <= RESPONSE_RESULT_IDX) {
    LOG(ERROR) << "Array in node is smaller than expected";
    return T();
  }

  mpack_node_t result = mpack_node_array_at(node, RESPONSE_RESULT_IDX);
  if (mpack_node_is_nil(result)) {
    if (!std::is_void<T>::value)
      LOG(WARNING) << "Got a nil result, but was expecting an actual value";
    return T();
  }

  return std::forward<T>(mpack_read<T>(result));
}

template <>
inline nvimrpc::Object nvimrpc::mpack_read<nvimrpc::Object>(mpack_node_t node) {
  nvimrpc::Object obj{};
  auto tag = mpack_node_tag(node);
  switch (auto type = mpack_tag_type(&tag)) {
  case mpack_type_bool: {
    return mpack_read<bool>(node);
  }
  case mpack_type_int: {
    return mpack_read<int64_t>(node);
  }
  case mpack_type_uint: {
    return (int64_t)(mpack_read<uint64_t>(node));
  }
  case mpack_type_double: {
    return mpack_read<double>(node);
  }
  case mpack_type_str: {
    return mpack_read<std::string>(node);
  }
  case mpack_type_map: {
    auto res_map =
        mpack_read<std::unordered_map<std::string, nvimrpc::Object>>(node);
    std::unordered_map<std::string, ObjectWrapper> res_map_wrapper;
    for (auto &item : res_map) {
      res_map_wrapper[item.first] = ObjectWrapper(item.second);
    }
    return nvimrpc::Object(res_map_wrapper);
  }
  case mpack_type_array: {
    auto res_vec = mpack_read<std::vector<nvimrpc::Object>>(node);
    std::vector<ObjectWrapper> res_vec_wrapper;
    for (auto &item : res_vec) {
      res_vec_wrapper.emplace_back(ObjectWrapper(item));
    }
    return nvimrpc::Object(res_vec_wrapper);
  }
  default: {
    LOG(ERROR) << "Unsupport type: '" << mpack_type_to_string(type) << "'";
    return {};
  }
  }
}

inline void nvimrpc::MpackRpcPack::set_method(std::string_view name) {
  if (name.empty()) {
    LOG(ERROR) << "Empty method name";
    return;
  }
  mpack_write_cstr(&writer, name.data());
}

inline void nvimrpc::mpack_write(mpack_writer_t *writer,
                                 const nvimrpc::Object &obj) {
  if (std::holds_alternative<bool>(obj))
    return mpack_write(writer, std::get<bool>(obj));
  if (std::holds_alternative<int64_t>(obj))
    return mpack_write(writer, std::get<int64_t>(obj));
  if (std::holds_alternative<double>(obj))
    return mpack_write(writer, std::get<double>(obj));
  if (std::holds_alternative<std::string>(obj))
    return mpack_write(writer, std::get<std::string>(obj));
  if (std::holds_alternative<std::array<int64_t, 2>>(obj))
    return mpack_write(writer, std::get<std::array<int64_t, 2>>(obj));
  if (std::holds_alternative<
          std::unordered_map<std::string, nvimrpc::ObjectWrapper>>(obj)) {
    return mpack_write(
        writer,
        std::get<std::unordered_map<std::string, nvimrpc::ObjectWrapper>>(obj));
  }
  if (std::holds_alternative<std::vector<nvimrpc::ObjectWrapper>>(obj)) {
    return mpack_write(writer,
                       std::get<std::vector<nvimrpc::ObjectWrapper>>(obj));
  }

  LOG(ERROR) << "Unrecognized nvimrpc::Object type!";
}

inline void nvimrpc::check_node_type(mpack_node_t node,
                                     mpack_type_t expected_type) {
  mpack_tag_t tag = mpack_node_tag(node);
  auto type = mpack_tag_type(&tag);
  if (type != expected_type)
    LOG(ERROR) << "Expected: '" << mpack_type_to_string(expected_type)
               << " node',  instead got: '" << mpack_type_to_string(type)
               << "'";
}

template <>
inline std::array<int64_t, 2>
nvimrpc::mpack_read<std::array<int64_t, 2>>(mpack_node_t node) {
  check_node_type(node, mpack_type_array);
  if (size_t l = mpack_node_array_length(node); l != 2) {
    LOG(ERROR)
        << ": Try to read std::array<int64_t, 2> while mpack array size is: '"
        << l << "'";
  }

  std::array<int64_t, 2> res;
  res[0] = mpack_read<int64_t>(mpack_node_array_at(node, 0));
  res[1] = mpack_read<int64_t>(mpack_node_array_at(node, 1));
  return res;
}
