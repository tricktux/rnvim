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
	const char *argv[] = {"gnvim",  "-n",           "/usr/bin/nvim", "--maximized",
		"--help", "--timeout=13", "file2.cpp",     "filee3.h"};

	int argc = sizeof(argv) / sizeof(char *);
	char **argv_ = new char *[argc];
	for (int k = 0; k < argc; k++) {
		argv_[k] = new char[32];
		std::strncpy(argv_[k], argv[k], 32);
	}

	cli::CliArgs args("gnvim", "BEST SOFTWARE EVER");
	std::string opt = args.get_arg("n,nvim", std::string());
	ASSERT_EQ(argv[2], opt);
	const std::vector<std::string> &pos = args.get_positional_arg();
	ASSERT_EQ(pos.size(), 2);
	for (int k = 6; k < argc; k++) {
		ASSERT_EQ(argv[k], pos[k - 6]);
	}
	bool max = args.get_arg("m,maximized", false);
	ASSERT_EQ(max, true);
	int t = args.get_arg("t,timeout", 0);
	ASSERT_EQ(t, 13);

	std::cout << args.get_help() << std::endl;

	for (int k = 0; k < argc; k++) {
		delete[] argv_[k];
	}
	delete[] argv_;
}
