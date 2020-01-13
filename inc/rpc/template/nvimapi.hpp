/// @file nvimapi.hpp
/// @brief High level controller of nvimrpc
/// @author Reinaldo Molina
/// @version  0.0
/// @date Dec 31 2019
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
// along with this program.  If not, see <http:www.gnu.org/licenses/>.

#ifndef NVIMAPI_HPP
#define NVIMAPI_HPP

#include "rpc/iodevice.hpp"
#include "rpc/msgpack.hpp"
#include "easylogging++.h"

#include <queue>
#include <string>
#include <vector>
#include <utility>
#include <unordered_map>

namespace nvimrpc {

class NvimApi {
  size_t msgid;
  std::string last_func_call;
  std::queue<mpack_node_t> pending_notif;
  IoDevice &device;
  IIoAsyncReader& async_stream;

	const static size_t POLL_TIMEOUT = 30; /// Seconds

  size_t get_new_msgid() { return ++msgid; }
  template <typename... Params>
  size_t dispatch(std::string_view func, Params &&... params) {
    if (func.empty()) {
      DLOG(ERROR) << "Empty function name";
      return 0;
    }

    last_func_call = func;
    size_t new_msgid = get_new_msgid();
    MpackRpcPack req_packer;
    req_packer.set_msgid(new_msgid);
    req_packer.set_method(func);
    req_packer.set_params(std::forward<Params>(params)...);
    std::string data = req_packer.build();

    if (data.empty()) {
      DLOG(ERROR) << "Failed to pack request";
      return 0;
    }

    if (size_t size = device.write(data); size != data.size()) {
      DLOG(ERROR) << "Failed to send all data";
      return 0;
    }

    return new_msgid;
  }

  template <typename T> T poll(size_t msgid) {
		DLOG(INFO) << "Waiting from response to request: '"
			<< last_func_call << "'";
    do {
      std::optional<std::vector<uint8_t>> rc = async_stream.poll(POLL_TIMEOUT);
      if (!rc) {
				DLOG_EVERY_N(100000, INFO) << "Not sufficient data in buffer";
        continue;
      }

			// TODO fix here
			mpack_node_t node;
      MpackRpcUnpack resp_unpack{node};

      if (size_t size = resp_unpack.get_num_elements();
          size != MpackRpcUnpack::RESPONSE_NUM_ELEMENTS) {
        DLOG(WARNING)
            << "Expected 4 elements while waiting for response from: '" << msgid
            << ":" << last_func_call << "', instead got size: '" << size << "'";
				if ((size == MpackRpcUnpack::NOTIFICATION_NUM_ELEMENTS) &&
						(resp_unpack.get_msg_type() ==
						 MpackRpcUnpack::NOTIFICATION_MSG_TYPE)) {

					auto redraw_node = mpack_node_array_at(node, 1);
					auto redraw_node_name = mpack_read<std::string>(redraw_node);
          DLOG(WARNING) << "Instead we got a notification: '"
						<< redraw_node_name << "'";
          pending_notif.push(node);

					auto event_group_node = mpack_node_array_at(node, 2);
					size_t event_group_length = mpack_node_array_length(event_group_node);

					for(size_t group_index = 0; group_index < event_group_length; ++group_index){
						auto event_array = mpack_node_array_at(event_group_node, group_index);
						// size_t event_array_length = mpack_node_array_length(event_array);

						auto name_node = mpack_node_array_at(event_array, 0);
						auto event_name = mpack_read<std::string>(name_node);

						DLOG(INFO) << "event_name: '" << event_name << "'";
						// for(size_t event_index = 1; event_index < event_array_length; ++event_index){
							// inn_dispatch_notif(event_name.c_str(), mpack_node_array_at(event_array, event_index), this);
						// }
					}
          continue;
        }

        return T();
      }

      if (int type = resp_unpack.get_msg_type();
					type != MpackRpcUnpack::RESPONSE_MSG_TYPE) {
        DLOG(WARNING) << "Packet received is not a RESPONSE: '" << type << "'";
        return T();
      }

      if (size_t id = resp_unpack.get_msgid(); id != msgid) {
        DLOG(WARNING) << "Received response, but not for my msgid: '" << id
                      << "'";
        continue;
      }

      if (std::optional<std::tuple<int64_t, std::string>>
					error = resp_unpack.get_error(); error.has_value()) {
				auto [code, msg] = error.value();
        DLOG(ERROR) << "RPC Server returned error. Error code: '" << code
					<< "'. Error msg: '" << msg << "'";
        return T();
      }

      return std::forward<T>(resp_unpack.get_result<T>());

    } while (true);
  }

public:
  explicit NvimApi(IoDevice &_device, IIoAsyncReader& reader)
      : msgid(0), device(_device), async_stream(reader) {}
  ~NvimApi() = default;
	// Generated apis
	// NOTE: poll function does not compile for poll<void>
	// Function get results just returns a default value if it detects a nil mpack 
	// return type
	// clang-format off
{% for req in nvim_reqs %}
	{{req.return_type}} {{req.name}}({% for arg in req.args %}{{arg.type}} {{arg.name}}{% if not loop.last %}, {% endif %}{% endfor %}) {
		{% if req.return_type != 'void' %}
			const size_t msgid = dispatch("{{req.name}}"{% for arg in req.args %}, {{arg.name}}{% endfor %});
					return poll<{{req.return_type}}>(msgid);
		{% else %}
		const size_t msgid = dispatch("{{req.name}}"{% for arg in req.args %}, {{arg.name}}{% endfor %});
			poll<int64_t>(msgid);
		{% endif %}
}
	{% endfor %}
	// clang-format on
};

} // namespace nvimrpc

#endif
