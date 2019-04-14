/// @file application.hpp
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

#pragma once

class Application {

	int parse_cli_args(int argc, char **argv);
public:
  Application() {}

  int init(int argc, char **argv);
  int run();
};
