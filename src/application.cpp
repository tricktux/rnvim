/// @file application.cpp
/// @brief Application class
/// @author Reinaldo Molina
/// @version  0.0
/// @date Apr 12 2019
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

#include "application.hpp"
#include <string>
#include "common.hpp"

/// @brief Initializes all members of the Application
/// @param argc Number of arguments passed to the application
/// @param argv Array of application arguments
/// @return SUCCESS, less than SUCCESS in case of failure
int Application::init(int argc, char **argv) {
	if (parse_cli_args(argc, argv) < SUCCESS)
		return -1;
	return SUCCESS;
}

/// @brief Parses command line arguments
/// see @CliArgs, and @CliParser
/// @param argc Number of arguments passed to the application
/// @param argv Array of application arguments
/// @return SUCCESS, less than SUCCESS in case of failure
int Application::parse_cli_args(int argc, char **argv) {
	// TODO: Create gflags pass under constructor argc and argv
	// TODO: pass a reference of gflags to CliArgs to get arguments
	if (argc < 1)
		return SUCCESS;

	std::string awe = { argv[1] };
	return SUCCESS;
}
