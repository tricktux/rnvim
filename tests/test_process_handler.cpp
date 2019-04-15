/// @file test_process_handler.cpp
/// @brief Tests for the process_handler class
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

#include "process_handler.hpp"
#include <gtest/gtest.h>

#include <string>
#include <vector>

TEST(process_handler, start_stop) {
  std::vector<std::string> cmd = {"nvim",       "-u",       "NONE",
                                  "--headless", "--listen", "127.0.0.1:6666"};
  ProcessHandler ph;
  ph.start(cmd, 1000);
  ASSERT_EQ(ph.is_running(), true);
  ph.stop(1000);
  ASSERT_EQ(ph.is_running(), false);
}
