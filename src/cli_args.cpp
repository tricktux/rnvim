/// @file cli_args.cpp
/// @brief Handle program arguments
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

#include "cxxopts.hpp"
#include "cli_args.hpp"

void CliArgs::init(ICliArgsGetter &getter) {
	for (auto &arg : str_args)
		arg.second = getter.get_arg(arg.first.c_str(), arg.second);
	for (auto &arg : int_args)
		arg.second = getter.get_arg(arg.first.c_str(), arg.second);
	for (auto &arg : bool_args)
		arg.second = getter.get_arg(arg.first.c_str(), arg.second);
}

int CliArgs::get_arg(const char *name, int def) {
	if ((name == nullptr) || (name[0] == 0))
		return def;

	const auto &search = int_args.find(name);

	if (search == int_args.end())
		return def;

	return search->second;
}
std::string CliArgs::get_arg(const char *name, const std::string &def) {
	if ((name == nullptr) || (name[0] == 0))
		return def;

	const auto &search = str_args.find(name);

	if (search == str_args.end())
		return def;

	return search->second;
}
bool CliArgs::get_arg(const char *name, bool def) {
	if ((name == nullptr) || (name[0] == 0))
		return def;

	const auto &search = bool_args.find(name);

	if (search == bool_args.end())
		return def;

	return search->second;
}
