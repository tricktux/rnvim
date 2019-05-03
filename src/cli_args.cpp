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
#include <tuple>
#include <utility>
#include <variant>
#include <vector>

#include "cli_args.hpp"
#include "common.hpp"
#include "cxxopts.hpp"
#include "easylogging/easylogging++.h"

int cli::CliArgs::parse(int argc, char **argv) {
  try {

    if (argv == nullptr) {
      DLOG(INFO) << "[" << __FUNCTION__ << "]: Invalid argv provided";
      return -1;
    }

    if (argc < 2) {
      DLOG(INFO) << "[" << __FUNCTION__ << "]: No arguments. Yay!!";
      return SUCCESS;
    }

    if (!pos_arg_name.empty()) {
      DLOG(INFO) << "[" << __FUNCTION__ << "]: Parsing positional arg: '"
                 << pos_arg_name << "'";
      opt.parse_positional(pos_arg_name.data());
    }
    opt.parse(argc, argv);

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
/// @param pos_argument - Tuple that contains, name, default, description and
/// help
/// @see @tuple_positional_args
void cli::CliArgs::add_pos_options(app::tuple_positional_args &pos_argument) {
  DLOG(INFO) << "[" << __FUNCTION__ << "]: called";

  if (std::get<0>(pos_argument).empty()) {
    DLOG(WARNING) << "[" << __FUNCTION__ << "]: Empty key";
    return;
  }

  DLOG(INFO) << "[" << __FUNCTION__ << "]: pos_argument: \n"
             << "\t" << std::get<0>(pos_argument) << "\n"
             << "\t" << std::get<2>(pos_argument) << "\n"
             << "\t" << std::get<3>(pos_argument);

  opt.add_options()(
      std::get<0>(pos_argument).data(), std::get<2>(pos_argument).data(),
      cxxopts::value<std::vector<std::string>>(std::get<1>(pos_argument)));

  opt.positional_help(std::get<3>(pos_argument).data());
  opt.show_positional_help();

  // Copy title of positional arg to be added during parse
  pos_arg_name = std::get<0>(pos_argument);
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
                      cxxopts::value<int>(arg.second.first)
                          ->default_value(std::to_string(arg.second.first)));
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
std::string_view cli::Options::get_arg(std::string_view name,
                                  std::string_view def) const {
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
