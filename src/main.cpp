/// @file main.cpp
/// @brief File description
/// @author Reinaldo Molina
/// @version  0.0
/// @date Mar 05 2019
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

#include <string>
#include <iostream>
#include <boost/lexical_cast.hpp>

int main(int argc, const char *argv[]) { 

	int count = 1;

	std::string yours = boost::lexical_cast<std::string>(count);

	std::cout << "yours:" << yours << std::endl;
	return 0;
}
