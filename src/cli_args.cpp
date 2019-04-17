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
#include "common.hpp"
#include "cxxopts.hpp"
#include "easylogging/easylogging++.h"

static Options opt;

// void CliArgs::init(ICliArgsGetter &getter) {
  // for (auto &arg : opt.str_args)
    // arg.second.first = getter.get_arg(arg.first.c_str(), arg.second.first);
  // for (auto &arg : opt.int_args)
    // arg.second.first = getter.get_arg(arg.first.c_str(), arg.second.first);
  // for (auto &arg : opt.bool_args)
    // arg.second.first = getter.get_arg(arg.first.c_str(), arg.second.first);
// }

int CliArgs::get_arg(std::string_view name, int def) const {
	if (name.empty())
		return def;

  const auto &search = opt.int_args.find(name.data());
  if (search == opt.int_args.end())
    return def;

  return search->second.first;
}
std::string CliArgs::get_arg(std::string_view name,
                             const std::string &def) const {
	if (name.empty())
		return def;

  const auto &search = opt.str_args.find(name.data());
  if (search == opt.str_args.end())
    return def;

  return search->second.first;
}
bool CliArgs::get_arg(std::string_view name, bool def) const {
	if (name.empty())
		return def;

  const auto &search = opt.bool_args.find(name.data());
  if (search == opt.bool_args.end())
    return def;

  return search->second.first;
}
const std::vector<std::string> &CliArgs::get_positional_arg() {
  return std::get<1>(opt.pos_arg);
}

/// @brief Parse program arguments
/// Results are stored in the static Options opt
/// They are then shared with @CliArgs which you can query
/// @param argc
/// @param argv
/// @return SUCCESS, or -100 in case of exception
int CxxOptsArgs::init(int argc, char **argv) {
  try {
    cxxopts::Options options(argv[0], opt.description.data());
    options.positional_help(opt.positional_help.data()).show_positional_help();

    for (auto &arg : opt.bool_args) {
      options.add_options()(
          arg.first, arg.second.second,
          cxxopts::value<bool>(arg.second.first)->implicit_value("true"));
    }
    for (auto &arg : opt.int_args) {
      options.add_options()(arg.first, arg.second.second,
                            cxxopts::value<int>(arg.second.first));
    }
    for (auto &arg : opt.str_args) {
      options.add_options()(arg.first, arg.second.second,
                            cxxopts::value<std::string>(arg.second.first));
    }

    // Add positional option
    options.add_options()(
        std::get<0>(opt.pos_arg), std::get<2>(opt.pos_arg),
        cxxopts::value<std::vector<std::string>>(std::get<1>(opt.pos_arg)));
    // Get positional option
    options.parse_positional(std::get<0>(opt.pos_arg));
    auto result = options.parse(argc, argv);
    // Get help string
    opt.help = options.help();

    return SUCCESS;
  } catch (const cxxopts::OptionException &e) {
    DLOG(ERROR) << "[" << __FUNCTION__
                << "]: Error parsing options: " << e.what();
    return -100;
  }
}
