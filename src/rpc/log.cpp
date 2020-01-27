/// @file log.cpp
/// @brief Source for logs
/// @author Reinaldo Molina
/// @version  0.0
/// @date Jan 27 2020

// Copyright © 2020 Reinaldo Molina

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

#include "easylogging++.h"
#include <filesystem>

class Log {
public:
  Log();
};

namespace fs = std::filesystem;
INITIALIZE_EASYLOGGINGPP

Log::Log() {
  fs::path elconf{fs::current_path()};
  if (elconf.empty()) {
    return;
  }

  elconf.append("easylogingpp.conf");
  if (!fs::is_regular_file(elconf)) {
    return;
  }
  el::Configurations conf(elconf.c_str());
  el::Loggers::reconfigureAllLoggers(conf);
  DLOG(INFO) << ">>>Start of Log<<<";
  DLOG(INFO) << "Easylogging config file: '" << elconf << "'";
}

static Log log{};
