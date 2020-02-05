/// @file log.hpp
/// @brief Header for logs
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

#ifndef LOG_HPP
#define LOG_HPP

#include "easylogging++.h"

class Log {
  const char *ENV_VAR = "NVIMPP_LOG_FILE";
  const char *FORMAT = "%datetime{%M-%d-%y %H:%m:%s,%g} %level %fbase: %msg";
  const char *SUBSECOND_PRECISION = "4";
  const char *PERFORMANCE_TRACKING = "false";
  const char *MAX_LOG_FILE_SIZE = "52428800"; // 50MB
  const char *LOG_FLUSH_THRESHOLD = "100";

public:
  Log();
};

#endif
