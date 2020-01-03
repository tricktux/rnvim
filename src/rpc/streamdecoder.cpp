/// @file streamdecoder.cpp
/// @brief Class in charge of parsing stream output
/// @author Reinaldo Molina
/// @version  0.0
/// @date Dec 19 2019
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

#include "rpc/streamdecoder.hpp"

/** 
 * @brief Parse data from the `stdout` into an `mpack_node_t`
 * This function uses `mpack_tree_try_parse` which in turn calls `read_iodev`, 
 * which in turn calls `iodevice->recv`, which then asynchronously retrieves 
 * data in `output` string.
 * `mpack_tree_root` converts the `mpack_tree_t` into a `mpack_node_t`
 * See @ref ReprocDevice::recv
 * @return `mpack_node_t`
 */
std::optional<mpack_node_t> nvimrpc::StreamDecoder::poll() {
  if (mpack_tree_try_parse(&tree)) {
    return mpack_tree_root(&tree);
  }

  // no message valid, two possible reasons:
  //   1. error occurred
  //   2. no sufficent data received
	mpack_error_t ec = mpack_tree_error(&tree);
	if (ec == mpack_ok) {
		DLOG(WARNING) << "Did not received expected count of data";
		return {};
	}

	std::string err{"Error: "};
	err.append(mpack_error_to_string(ec));
	throw std::runtime_error(err.c_str());
	DLOG(ERROR) << err;
	return {};
}
