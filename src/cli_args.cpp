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

#include <string>
#include <utility>
#include <vector>

#include "cli_args.hpp"
#include "common.hpp"
#include "cxxopts.hpp"
#include "easylogging/easylogging++.h"

int cli::CliArgs::parse_and_save_options(int argc, char **argv,
                                         cli::Options &opt_save) {
  try {

    if (argc < 2) {
      DLOG(INFO) << "[" << __FUNCTION__ << "]: No arguments. Yay!!";
      return SUCCESS;
    }

    opt.parse_positional(pos_arg.data());
    auto res = opt.parse(argc, argv);

		const char *p;
		for (auto &arg : opt_save.str_args) {
			p = arg.first.data();
			if (res.count(p)) {
				arg.second.first = res[p].as<std::string>();
			}
		}
		for (auto &arg : opt_save.int_args) {
			p = arg.first.data();
			if (res.count(p)) {
				arg.second.first = res[p].as<int>();
			}
		}
		for (auto &arg : opt_save.bool_args) {
			p = arg.first.data();
			if (res.count(p)) {
				arg.second.first = res[p].as<bool>();
			}
		}
		p = std::get<0>(opt_save.pos_arg).data();
		if (res.count(p)) {
			opt_save.set_pos_arg(res[p].as<std::vector<std::string>>());
		}

    return SUCCESS;
  } catch (const cxxopts::OptionException &excep) {
    DLOG(ERROR) << "[" << __FUNCTION__
                << "]: Error parsing options: " << excep.what();
    return -100;
  }
}

/// @brief Adds argument of type positional
/// Also adds the positional help, enables library to show the positional help
/// and saves argument name to be used in parse function
/// @param pos_arg - Tuple that contains, name, default, description and help
/// @see @tuple_positional_args
void cli::CliArgs::add_pos_options(app::tuple_positional_args &pos_arg) {
  if (std::get<0>(pos_arg).empty()) {
    DLOG(WARNING) << "[" << __FUNCTION__ << "]: Empty key";
    return;
  }

  opt.add_options()(
      std::get<0>(pos_arg).data(), std::get<2>(pos_arg).data(),
      cxxopts::value<std::vector<std::string>>(std::get<1>(pos_arg)));

  opt.positional_help(std::get<3>(pos_arg).data());
  opt.show_positional_help();

  // Copy title of positional arg to be added during parse
  this->pos_arg = std::get<0>(pos_arg);
}

void cli::CliArgs::add_options(app::map_bool_args &bool_args) {
  if (bool_args.empty()) {
    DLOG(WARNING) << "[" << __FUNCTION__ << "]: Empty map of options detected";
    return;
  }

  for (auto &arg : bool_args) {
    if (arg.first.empty()) {
      DLOG(WARNING) << "[" << __FUNCTION__ << "]: Empty option name detected";
      continue;
    }

    opt.add_options()(
        arg.first.data(), arg.second.second.data(),
        cxxopts::value<bool>(arg.second.first)->implicit_value("true"));
  }
}

void cli::CliArgs::add_options(app::map_int_args &int_args) {
  if (int_args.empty()) {
    DLOG(WARNING) << "[" << __FUNCTION__ << "]: Empty map of options detected";
    return;
  }

  for (auto &arg : int_args) {
    if (arg.first.empty()) {
      DLOG(WARNING) << "[" << __FUNCTION__ << "]: Empty option name detected";
      continue;
    }

    opt.add_options()(arg.first.data(), arg.second.second.data(),
                      cxxopts::value<int>(arg.second.first));
  }
}

void cli::CliArgs::add_options(app::map_string_args &string_args) {
  if (string_args.empty()) {
    DLOG(WARNING) << "[" << __FUNCTION__ << "]: Empty map of options detected";
    return;
  }

  for (auto &arg : string_args) {
    if (arg.first.empty()) {
      DLOG(WARNING) << "[" << __FUNCTION__ << "]: Empty option name detected";
      continue;
    }

    opt.add_options()(arg.first.data(), arg.second.second.data(),
                      cxxopts::value<std::string>(arg.second.first));
  }
}

int cli::Options::get_arg(std::string_view name, int def) const {
  if (name.empty())
    return def;

  const auto &search = int_args.find(name.data());
  if (search == int_args.end())
    return def;

  return search->second.first;
}
std::string cli::Options::get_arg(std::string_view name,
                                  const std::string &def) const {
  if (name.empty())
    return def;

  const auto &search = str_args.find(name.data());
  if (search == str_args.end())
    return def;

  return search->second.first;
}

bool cli::Options::get_arg(std::string_view name, bool def) const {
  if (name.empty())
    return def;

  const auto &search = bool_args.find(name.data());
  if (search == bool_args.end())
    return def;

  return search->second.first;
}

void cli::Options::set_arg(std::string_view name, int val) {
  if (name.empty())
    return;

  auto search = int_args.find(name.data());
  if (search == int_args.end())
    return;

  search->second.first = val;
}
void cli::Options::set_arg(std::string_view name, std::string_view val) {
  if (name.empty())
    return;

  auto search = str_args.find(name.data());
  if (search == str_args.end())
    return;

  search->second.first = val;
}

void cli::Options::set_arg(std::string_view name, bool val) {
  if (name.empty())
    return;

  auto search = bool_args.find(name.data());
  if (search == bool_args.end())
    return;

  search->second.first = val;
}
