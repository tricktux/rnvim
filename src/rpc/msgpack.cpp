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

int nvimrpc::MpackRpcUnpack::get_error() {
  const mpack_tag_t error = mpack_peek_tag(&reader);
  if (error.type == mpack_type_nil) {
    mpack_discard(&reader); // Disregard the error element
    return 0;
  }

  if (error.type == mpack_type_str) {
    char *buf = mpack_expect_cstr_alloc(&reader, MAX_CSTR_SIZE);
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

