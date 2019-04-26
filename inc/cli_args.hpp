/// @file cli_args.hpp
/// @brief Handling of application arguments
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

#include "cxxopts.hpp"

#include <string>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <vector>

namespace app {
typedef std::unordered_map<std::string_view,
                           std::pair<std::string, std::string_view>>
    map_string_args;
typedef std::unordered_map<std::string_view, std::pair<int, std::string_view>>
    map_int_args;
typedef std::unordered_map<std::string_view, std::pair<bool, std::string_view>>
    map_bool_args;
typedef std::tuple<std::string_view, std::vector<std::string>, std::string_view,
                   std::string_view>
    tuple_positional_args;

} // namespace app
namespace cli {
constexpr std::string_view PROGRAM_NAME = "gnvim";
constexpr std::string_view PROGRAM_DESCRIPTION = "gnvim - GUI for neovim";
constexpr std::string_view PROGRAM_VERSION = "gnvim - GUI for neovim\nv0.0.1";

constexpr std::string_view STR_ARG_NVIM_OPTS = "a,nvim_args";
constexpr std::string_view STR_ARG_NVIM_OPTS_DEFAULT = "";
constexpr std::string_view STR_ARG_NVIM_OPTS_DESCRIPTION =
    "Options to pass to neovim";

constexpr std::string_view STR_ARG_NVIM = "n,nvim";
constexpr std::string_view STR_ARG_NVIM_DEFAULT = "nvim";
constexpr std::string_view STR_ARG_NVIM_DESCRIPTION = "nvim executable path";

constexpr std::string_view INT_ARG_TIMEOUT = "t,timeout";
constexpr int INT_ARG_TIMEOUT_DEFAULT = 15;
constexpr std::string_view INT_ARG_TIMEOUT_DESCRIPTION =
    "Error if nvim does not responde after count seconds";

constexpr std::string_view BOOL_ARG_MAXIMIZED = "m,maximized";
constexpr bool BOOL_ARG_MAXIMIZED_DEFAULT = false;
constexpr std::string_view BOOL_ARG_MAXIMIZED_DESCRIPTION =
    "Maximize the window on startup";

constexpr std::string_view BOOL_ARG_HELP = "h,help";
constexpr bool BOOL_ARG_HELP_DEFAULT = false;
constexpr std::string_view BOOL_ARG_HELP_DESCRIPTION = "Print this help";

constexpr std::string_view BOOL_ARG_VERSION = "v,version";
constexpr bool BOOL_ARG_VERSION_DEFAULT = false;
constexpr std::string_view BOOL_ARG_VERSION_DESCRIPTION =
    "Print version information";
constexpr std::string_view STR_POS_ARG = "files";
const std::vector<std::string> STR_POS_ARG_DEFAULT = {};
constexpr std::string_view STR_POS_ARG_DESCRIPTION = "List of files to open";
constexpr std::string_view STR_POS_ARG_HELP = "[optional list of files]";

/// Interface to get argument options
class ICliArgsGetter {
public:
  virtual ~ICliArgsGetter() {}
  virtual void add_options(app::map_string_args &string_args) = 0;
  virtual void add_options(app::map_int_args &int_args) = 0;
  virtual void add_options(app::map_bool_args &bool_args) = 0;
  virtual void add_pos_options(app::tuple_positional_args &pos_arg) = 0;
  // virtual int parse_options(int argc, char **argv) = 0;
  // virtual int get_arg(std::string_view name, int def) const = 0;
  // virtual std::string get_arg(std::string_view name,
  // const std::string &def) const = 0;
  // virtual bool get_arg(std::string_view name, bool def) const = 0;
  virtual std::string_view get_help() const = 0;
  // virtual std::string_view get_version() const = 0;
};

class Options {
public:
  /// Option name, {option value, option help text}
  app::map_string_args str_args;
  app::map_int_args int_args;
  app::map_bool_args bool_args;

  /// Storage for poitional args
  /// Option name, storage for values, description, help
  app::tuple_positional_args pos_arg;

  /// Contains information to show when using -h
  Options()
      : str_args({{STR_ARG_NVIM,
                   {STR_ARG_NVIM_DEFAULT.data(), STR_ARG_NVIM_DESCRIPTION}},
                  {STR_ARG_NVIM_OPTS,
                   {STR_ARG_NVIM_OPTS_DEFAULT.data(),
                    STR_ARG_NVIM_OPTS_DESCRIPTION}}}),
        int_args({{INT_ARG_TIMEOUT,
                   {INT_ARG_TIMEOUT_DEFAULT, INT_ARG_TIMEOUT_DESCRIPTION}}}),
        bool_args(
            {{BOOL_ARG_MAXIMIZED,
              {BOOL_ARG_MAXIMIZED_DEFAULT, BOOL_ARG_MAXIMIZED_DESCRIPTION}},
             {BOOL_ARG_HELP,
              {BOOL_ARG_HELP_DEFAULT, BOOL_ARG_HELP_DESCRIPTION}},
             {BOOL_ARG_VERSION,
              {BOOL_ARG_VERSION_DEFAULT, BOOL_ARG_VERSION_DESCRIPTION}}}),
        pos_arg({STR_POS_ARG, STR_POS_ARG_DEFAULT, STR_POS_ARG_DESCRIPTION,
                 STR_POS_ARG_HELP}) {}
  int get_arg(std::string_view name, int def) const;
  std::string get_arg(std::string_view name, const std::string &def) const;
  bool get_arg(std::string_view name, bool def) const;
	const std::vector<std::string>& get_pos_arg() {
		return std::get<1>(pos_arg);
	}

  std::string_view get_version() const { return PROGRAM_VERSION; };

	// TODO = not really used
  void set_arg(std::string_view name, int val);
  void set_arg(std::string_view name, std::string_view val);
  void set_arg(std::string_view name, bool val);
	void set_pos_arg(const std::vector<std::string> &val) {
		std::get<1>(pos_arg) = val;
	}

};

// TODO This must belong to app::
// static Options options_def;

/// Local interface to get options
/// Provides abstraction from external libraries
class CliArgs : public ICliArgsGetter {
  /// There can be only one positional argument per program design
  std::string_view pos_arg;
  cxxopts::Options opt;

public:
  CliArgs(std::string_view program_name, std::string_view program_description)
      : opt(program_name.data(), program_description.data()) {}
  virtual ~CliArgs() {}

	// TODO remove the map_{}_args and just get the names and descriptions
  void add_options(app::map_string_args &string_args) override;
  void add_options(app::map_int_args &int_args) override;
  void add_options(app::map_bool_args &bool_args) override;
  void add_pos_options(app::tuple_positional_args &pos_argument) override;
  std::string_view get_help() const override { return opt.help({""}); }

	int parse(int argc, char **argv);
};

} // namespace cli
