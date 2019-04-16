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

#include "cli_args.hpp"
#include "cxxopts.hpp"
#include "easylogging/easylogging++.h"

static Options opt;

void CliArgs::init(ICliArgsGetter &getter) {
  for (auto &arg : opt.str_args)
    arg.second.first = getter.get_arg(arg.first.c_str(), arg.second.first);
  for (auto &arg : opt.int_args)
    arg.second.first = getter.get_arg(arg.first.c_str(), arg.second.first);
  for (auto &arg : opt.bool_args)
    arg.second.first = getter.get_arg(arg.first.c_str(), arg.second.first);
}

int CliArgs::get_arg(const char *name, int def) {
  if ((name == nullptr) || (name[0] == 0))
    return def;

  const auto &search = opt.int_args.find(name);
  if (search == opt.int_args.end())
    return def;

  return search->second.first;
}
std::string CliArgs::get_arg(const char *name, const std::string &def) {
  if ((name == nullptr) || (name[0] == 0))
    return def;

  const auto &search = opt.str_args.find(name);
  if (search == opt.str_args.end())
    return def;

  return search->second.first;
}
bool CliArgs::get_arg(const char *name, bool def) {
  if ((name == nullptr) || (name[0] == 0))
    return def;

  const auto &search = opt.bool_args.find(name);
  if (search == opt.bool_args.end())
    return def;

  return search->second.first;
}

void CxxOptsArgs::init(int argc, char **argv) {
  try {
    cxxopts::Options options(argv[0], opt.description);
    options.positional_help(opt.positional_help).show_positional_help();

		auto result = options.parse(argc, argv);

  } catch (const cxxopts::OptionException &e) {
    DLOG(ERROR) << "[" << __FUNCTION__
                << "]: Error parsing options: " << e.what();
    return;
  }
}
