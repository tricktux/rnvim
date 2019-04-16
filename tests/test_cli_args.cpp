/// @file test_cli_args.cpp
/// @brief Tests for cli args interface
/// @author Reinaldo Molina
/// @version  0.0
/// @date Apr 16 2019
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

#include "cli_args.hpp"
#include <gtest/gtest.h>

#include "easylogging/easylogging++.h"

INITIALIZE_EASYLOGGINGPP

TEST(cxxopts_args, loading) {
	std::cout << "Cool" << std::endl;
}
