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
constexpr std::string_view PROGRAM_DESCRIPTION = "gnvim - GUI for neovim";

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
  virtual int init(std::string_view program_name,
                   std::string_view program_description) = 0;
  virtual void add_options(const app::map_string_args &string_args) = 0;
  virtual void add_options(const app::map_int_args &int_args) = 0;
  virtual void add_options(const app::map_bool_args &bool_args) = 0;
	virtual void add_pos_options(const app::tuple_positional_args &pos_arg) = 0;
  virtual int parse_options(int argc, char **argv) = 0;
  virtual int get_arg(std::string_view name, int def) const = 0;
  virtual std::string get_arg(std::string_view name,
                              const std::string &def) const = 0;
  virtual bool get_arg(std::string_view name, bool def) const = 0;
  virtual std::string_view get_help() const = 0;
  virtual std::string_view get_version() const = 0;
};

typedef struct _Options {
  /// Option name, {option value, option help text}
  app::map_string_args str_args;
  app::map_int_args int_args;
  app::map_bool_args bool_args;

  /// Storage for poitional args
	/// Option name, storage for values, description, help
  app::tuple_positional_args pos_arg;

  /// Contains information to show when using -h
  std::string help;
  // TODO- Create a version string

  _Options()
      : str_args({{STR_ARG_NVIM,
                   {STR_ARG_NVIM_DEFAULT.data(), STR_ARG_NVIM_DESCRIPTION}}}),
        int_args({{INT_ARG_TIMEOUT,
                   {INT_ARG_TIMEOUT_DEFAULT, INT_ARG_TIMEOUT_DESCRIPTION}}}),
        bool_args(
            {{BOOL_ARG_MAXIMIZED,
              {BOOL_ARG_MAXIMIZED_DEFAULT, BOOL_ARG_MAXIMIZED_DESCRIPTION}},
             {BOOL_ARG_HELP,
              {BOOL_ARG_HELP_DEFAULT, BOOL_ARG_HELP_DESCRIPTION}},
             {BOOL_ARG_VERSION,
              {BOOL_ARG_VERSION_DEFAULT, BOOL_ARG_VERSION_DESCRIPTION}}}),
        pos_arg({STR_POS_ARG, STR_POS_ARG_DEFAULT, STR_POS_ARG_DESCRIPTION}) {}
} Options;

/// Local interface to get options
/// Provides abstraction from external libraries
class CliArgs : public ICliArgsGetter {
  /// There can be only one positional argument per program design
	std::string_view pos_arg;
  Options options_def;
  cxxopts::Options opt;

public:
  CliArgs(std::string_view program_name, std::string_view program_description)
      : options_def(), opt(program_name.data(), program_description.data()) {}
  virtual ~CliArgs() {}

  void add_options(const app::map_string_args &string_args) override;
  void add_options(const app::map_int_args &int_args) override;
  void add_options(const app::map_bool_args &bool_args) override;
	void add_pos_options(const app::tuple_positional_args &pos_arg) override;
  int get_arg(std::string_view name, int def) const override;
  std::string get_arg(std::string_view name,
                      const std::string &def) const override;
  bool get_arg(std::string_view name, bool def) const override;
  const std::vector<std::string> &get_positional_arg();
  std::string_view get_help() const override;
  std::string_view get_version() const override;
};

/// Implementation that parses options and makes them available
// TODO-[RM]-(Wed Apr 17 2019 15:29):
// Get rid of this
class CxxOptsArgs {

public:
  CxxOptsArgs() {}
  virtual ~CxxOptsArgs() {}

  int init(int argc, char **argv);
};
} // namespace cli
