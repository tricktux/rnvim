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

#include "easylogging++.h"
#include "rpc/msgpack.hpp"
#include "nvimapi.hpp"

#include <chrono>
#include <memory>
#include <string>
#include <string_view>
#include <utility>
#include <optional>

template <typename... Params>
size_t nvimrpc::NvimApi::dispatch(std::string func, Params &&... params) {
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

// TODO probably disregarding MpackResUnPack will cause a crash
template <typename T> T nvimrpc::NvimApi::poll(size_t msgid, size_t timeout) {
  do {
    std::string buf{};
    device.recv(buf, std::chrono::seconds{timeout});
    if (buf.empty()) {
      DLOG(ERROR) << "Timed out waiting for '" << msgid << "'";

      if constexpr (std::is_void<T>::value)
        return;
      return T{};
    }

    MpackResUnPack resp_unpack;
    if (int rc = resp_unpack.set_data(buf); rc != 0) {
      DLOG(WARNING) << "Failed to make sense of data received: '" << buf << "'";
      continue; // Keep trying?
    }

    if (int rc = resp_unpack.get_msg_type(); rc != MpackResUnPack::MSG_TYPE) {
      DLOG(WARNING) << "Packet received is not a RESPONSE: '" << rc << "'";
      if (rc == 2)               // TODO Address magic number
        pending_notif.push(buf); // Save notification data

      continue;
    }

    if (size_t rc = resp_unpack.get_msgid(); rc != msgid) {
      DLOG(WARNING) << "Received response, but not for my msgid: '" << rc
                    << "'";
      continue;
    }

    if (int rc = resp_unpack.get_error(); rc != 0) {
      DLOG(ERROR) << "Server returned error: '" << rc << "'";
      if constexpr (std::is_void<T>::value)
        return;
      return T{};
    }

    if constexpr (std::is_void<T>::value)
      return;
    return std::forward<T>(resp_unpack.get_result<T>());

  } while (true);
}

template <> void nvimrpc::NvimApi::poll<void>(size_t msgid, size_t timeout) {
  do {
    std::string buf{};
    device.recv(buf, std::chrono::seconds{timeout});
    if (buf.empty()) {
      DLOG(ERROR) << "Timed out waiting for '" << msgid << "'";

      return;
    }

    MpackResUnPack resp_unpack;
    if (int rc = resp_unpack.set_data(buf); rc != 0) {
      DLOG(WARNING) << "Failed to make sense of data received: '" << buf << "'";
      continue; // Keep trying?
    }

    if (int rc = resp_unpack.get_msg_type(); rc != MpackResUnPack::MSG_TYPE) {
      DLOG(WARNING) << "Packet received is not a RESPONSE: '" << rc << "'";
      if (rc == 2)               // TODO Address magic number
        pending_notif.push(buf); // Save notification data

      continue;
    }

    if (size_t rc = resp_unpack.get_msgid(); rc != msgid) {
      DLOG(WARNING) << "Received response, but not for my msgid: '" << rc
                    << "'";
      continue;
    }

    if (int rc = resp_unpack.get_error(); rc != 0) {
      DLOG(ERROR) << "Server returned error: '" << rc << "'";
      return;
    }

    return;

  } while (true);
}
