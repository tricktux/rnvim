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

#include "rpc/streamdecoder.hpp"
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
  StreamDecoder decoder;

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

    if (size_t size = device.send(data); size != data.size()) {
      DLOG(ERROR) << "Failed to send all data";
      return 0;
    }

    return new_msgid;
  }

  template <typename T> T poll(size_t msgid) {
    do {
      std::optional<mpack_node_t> rc = decoder.poll();
      if (!rc) {
				DLOG_EVERY_N(100000, INFO) << "Not sufficient data in buffer";
				// std::this_thread::sleep_for(std::chrono::microseconds{50});
        continue;
      }
      MpackRpcUnpack resp_unpack{rc.value()};

      if (size_t size = resp_unpack.get_num_elements();
          size != MpackRpcUnpack::RESPONSE_NUM_ELEMENTS) {
        DLOG(WARNING)
            << "Expected 4 elements while waiting for response from: '" << msgid
            << ":" << last_func_call << "', instead got size: '" << size << "'";
				if ((size == MpackRpcUnpack::NOTIFICATION_NUM_ELEMENTS) &&
						(resp_unpack.get_msg_type() ==
						 MpackRpcUnpack::NOTIFICATION_MSG_TYPE)) {

					auto redraw_node = mpack_node_array_at(rc.value(), 1);
					auto redraw_node_name = mpack_read<std::string>(redraw_node);
          DLOG(WARNING) << "Instead we got a notification: '"
						<< redraw_node_name << "'";
          pending_notif.push(rc.value());

					auto event_group_node = mpack_node_array_at(rc.value(), 2);
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

      if (int rc = resp_unpack.get_msg_type();
					rc != MpackRpcUnpack::RESPONSE_MSG_TYPE) {
        DLOG(WARNING) << "Packet received is not a RESPONSE: '" << rc << "'";
        return T();
      }

      if (size_t rc = resp_unpack.get_msgid(); rc != msgid) {
        DLOG(WARNING) << "Received response, but not for my msgid: '" << rc
                      << "'";
        continue;
      }

      if (std::optional<std::tuple<int64_t, std::string>>
					rc = resp_unpack.get_error(); rc.has_value()) {
				auto [code, msg] = rc.value();
        DLOG(ERROR) << "RPC Server returned error. Error code: '" << code
					<< "'. Error msg: '" << msg << "'";
        return T();
      }

      return std::forward<T>(resp_unpack.get_result<T>());

    } while (true);
  }
  // template <> void poll<void>(size_t msgid, size_t timeout);

public:
  explicit NvimApi(IoDevice &_device)
      : msgid(0), device(_device), decoder(_device) {}
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


// template <> void nvimrpc::NvimApi::poll(size_t msgid, size_t timeout) {
	// do {
		// std::string buf{};
		// if (size_t size = device.recv(buf, timeout); size == 0) {
			// DLOG(ERROR) << "Timed out waiting for '" << msgid << "'";

			// return;
		// }

		// MpackRpcUnpack resp_unpack;
		// if (int rc = resp_unpack.set_data(buf); rc != 0) {
			// DLOG(WARNING) << "Failed to make sense of data received: '" << buf << "'";
			// continue; // Keep trying?
		// }

		// if (int rc = resp_unpack.get_msg_type(); rc != MpackRpcUnpack::MSG_TYPE) {
			// DLOG(WARNING) << "Packet received is not a RESPONSE: '" << rc << "'";
			// if (rc == 2)               // TODO Address magic number
				// pending_notif.push(buf); // Save notification data

			// continue;
		// }

		// if (size_t rc = resp_unpack.get_msgid(); rc != msgid) {
			// DLOG(WARNING) << "Received response, but not for my msgid: '" << rc
				// << "'";
			// continue;
		// }

		// if (int rc = resp_unpack.get_error(); rc != 0) {
			// DLOG(ERROR) << "Server returned error: '" << rc << "'";
			// return;
		// }

		// return;

	// } while (true);
// }

#endif
