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

#include "rpc/log.hpp"
#include <filesystem>
#include <cstdlib>

namespace fs = std::filesystem;
INITIALIZE_EASYLOGGINGPP

Log::Log() {
  bool enabled = false;
  const char *log_path = std::getenv(ENV_VAR);
  if (log_path != nullptr) {
    fs::path lpath{log_path};
    if (fs::exists(lpath.parent_path())) {
      enabled = true;
    }
  }

  el::Configurations conf;
  conf.setToDefault();
  // Values are always std::string
  conf.setGlobally(el::ConfigurationType::Format, FORMAT);
  if (enabled) {
    conf.setGlobally(el::ConfigurationType::Filename, log_path);
  }
  conf.setGlobally(el::ConfigurationType::SubsecondPrecision,
                   SUBSECOND_PRECISION);
  conf.setGlobally(el::ConfigurationType::PerformanceTracking,
                   PERFORMANCE_TRACKING);
  conf.setGlobally(el::ConfigurationType::MaxLogFileSize, MAX_LOG_FILE_SIZE);
  conf.setGlobally(el::ConfigurationType::LogFlushThreshold,
                   LOG_FLUSH_THRESHOLD);
  conf.setGlobally(el::ConfigurationType::ToStandardOutput, "false");
  conf.setGlobally(el::ConfigurationType::Enabled, enabled ? "true" : "false");
  el::Loggers::reconfigureAllLoggers(conf);
  LOG(INFO) << ">>>Start of Log<<<";
}

static Log log{};
