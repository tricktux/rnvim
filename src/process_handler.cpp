/// @file process_handler.cpp
/// @brief Responsible for handling the neovim process
/// @author Reinaldo Molina
/// @version  0.0
/// @date Apr 15 2019
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

#include "common.hpp"
#include "easylogging/easylogging++.h"
#include "process_handler.hpp"

/// @brief Starts the process provided in @cmd
/// @param cmd Command to execute, includes arguments
/// @param timeout timeout in milliseconds
/// @return SUCCESS or <SUCCESS in case of error
int ProcessHandler::start(const std::vector<std::string> &cmd,
                          unsigned int timeout) {
  std::error_code ec;
  if (cmd.empty()) {
    DLOG(ERROR) << "[ProcessHandler::start]: cmd argument empty";
    return -1;
  }

  if (timeout < 0) {
    DLOG(WARNING) << "[ProcessHandler::start]: invalid timeout (" << timeout
                  << ")";
    timeout = 0;
  }

  this->timeout = timeout;

  DLOG(INFO) << "[ProcessHandler::start]: Cmd = ";
  for (const auto &c : cmd)
    DLOG(INFO) << "\t" << c;

  nvim = reproc::process(reproc::terminate, reproc::milliseconds(timeout / 2),
                         reproc::kill, reproc::milliseconds(timeout / 2));

  ec = nvim.start(cmd);
  if ((ec = nvim.start(cmd))) {
    DLOG(ERROR) << "[ProcessHandler::start]: Failed to start process."
                << " ec.value = '" << ec.value() << "'."
                << " ec.message = '" << ec.value() << "'";
    if (ec == reproc::errc::file_not_found)
      DLOG(ERROR)
          << "[ProcessHandler::start]: "
          << "Program not found. Make sure it's available from the PATH.";
    return -2;
  }

  // TODO-[RM]-(Mon Apr 15 2019 15:08):
  // - Is this needed?
  nvim.close(reproc::stream::in);

  return ec.value();
}
