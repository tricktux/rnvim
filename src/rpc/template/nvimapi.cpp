/// @file nvimapi.cpp
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

#include "rpc/template/nvimapi.hpp"
#include "rpc/msgpack.hpp"
#include "easylogging++.h"

#include <memory>

template <typename... Params>
size_t nvimrpc::NvimApi::dispatch(std::string_view func, Params&& ... params) {
	if (func.empty()) {
		DLOG(ERROR) << "Empty function name";
		return 0;
	}

	size_t new_msgid = get_new_msgid();
	MPackReqPack req_packer;
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
