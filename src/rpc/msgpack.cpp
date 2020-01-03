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
    DLOG(ERROR) << "Error flushing and closing the underlying stream";
		DLOG(ERROR) << "Error: '" << mpack_error_to_string( error) << "'";
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
    DLOG(ERROR) << "Empty main node";
    return {};
  }
  if (mpack_node_array_length(node) <= RESPONSE_ERROR_IDX) {
    DLOG(ERROR) << "Array in node is smaller than expected";
    return {};
  }

  mpack_node_t error = mpack_node_array_at(node, RESPONSE_ERROR_IDX);
  if (mpack_node_is_nil(error)) // No error to report
    return {};

  if (size_t errnode_size = mpack_node_array_length(error); errnode_size != 2) {
    DLOG(ERROR) << ": RESP error array [type, message] has invalid size: '"
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
	DLOG(INFO) << log_str;
}
