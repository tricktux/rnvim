/// @file streamdecoder.hpp
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

#ifndef STREAMDECODER_HPP
#define STREAMDECODER_HPP

#include "nvimpp/log.hpp"
#include "mpack.h"
#include "nvimpp/iodevice.hpp"
#include <optional>

namespace nvimrpc {

/** @brief Wrap around `MPack` Node and Stream capabilites
 * See
 * [here](https://github.com/ludocode/mpack/blob/develop/docs/node.mdhttps://github.com/ludocode/mpack/blob/develop/docs/node.md)
 * */
class StreamDecoder {
	// const size_t MAX_NODES = 4096;
	// const size_t MAX_SIZE = MAX_NODES*1024;
	const size_t MAX_NODES = SIZE_MAX;
	const size_t MAX_SIZE = SIZE_MAX;
  mpack_tree_t tree; /// Gets parsed into a node

  /**
   * @brief Function used by mpack to read bytes from `stdout`
   * This function is passed to `mpack_tree_init_stream`, so that every time
   * `mpack_tree_try_parse` is called, this function is the one that provides
   * the bytes.  According to `MPack` documentation in order to use
   * `mpack_tree_try_parse` this function needs to be `async`
   * @param ptree
   * @param buf
   * @param count
   * @return
   */
  static size_t read_iodev(mpack_tree_t *ptree, char *buf, size_t count);

public:
  StreamDecoder(IoDevice &dev) : tree() {
    mpack_tree_init_stream(&tree, &read_iodev, &dev, MAX_SIZE, MAX_NODES);
  }
  ~StreamDecoder() { mpack_tree_destroy(&tree); }

  std::optional<mpack_node_t> poll();
};

} // namespace nvimrpc

#endif
