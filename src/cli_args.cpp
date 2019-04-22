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
#include <vector>
#include <utility>

#include "cli_args.hpp"
#include "common.hpp"
#include "cxxopts.hpp"
#include "easylogging/easylogging++.h"

cxxopts::ParseResult&& cli::CliArgs::parse_options(int argc, char **argv) {
  try {

		// if (argc < 2) {
			// DLOG(INFO) << "[" << __FUNCTION__ << "]: No arguments. Yay!!";
			// return SUCCESS;
		// }

		opt.parse_positional(pos_arg.data());
		return std::move(opt.parse(argc, argv));


		// return SUCCESS;
  } catch (const cxxopts::OptionException &excep) {
    DLOG(ERROR) << "[" << __FUNCTION__
                << "]: Error parsing options: " << excep.what();
    // return -100;
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

/// @brief Parse program arguments
/// They are then shared with @cli::CliArgs which you can query
/// @param argc
/// @param argv
/// @return SUCCESS, or -100 in case of exception
// int cli::CxxOptsArgs::init(int argc, char **argv) {
// try {
// cxxopts::Options options(argv[0], opt.description.data());
// options.positional_help(opt.positional_help.data()).show_positional_help();

// for (auto &arg : opt.bool_args) {
// options.add_options()(
// arg.first, arg.second.second,
// cxxopts::value<bool>(arg.second.first)->implicit_value("true"));
// }
// for (auto &arg : opt.int_args) {
// options.add_options()(arg.first, arg.second.second,
// cxxopts::value<int>(arg.second.first));
// }
// for (auto &arg : opt.str_args) {
// options.add_options()(arg.first, arg.second.second,
// cxxopts::value<std::string>(arg.second.first));
// }

// // Add positional option
// options.add_options()(
// std::get<0>(opt.pos_arg), std::get<2>(opt.pos_arg),
// cxxopts::value<std::vector<std::string>>(std::get<1>(opt.pos_arg)));
// // Get positional option
// options.parse_positional(std::get<0>(opt.pos_arg));
// auto result = options.parse(argc, argv);
// // Get help string
// opt.help = options.help();

// return SUCCESS;
// } catch (const cxxopts::OptionException &e) {
// DLOG(ERROR) << "[" << __FUNCTION__
// << "]: Error parsing options: " << e.what();
// return -100;
// }
// }
