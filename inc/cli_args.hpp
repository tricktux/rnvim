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

#include <string>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <vector>

typedef std::unordered_map<std::string, std::pair<std::string, std::string>>
    map_string_args;
typedef std::unordered_map<std::string, std::pair<int, std::string>>
    map_int_args;
typedef std::unordered_map<std::string, std::pair<bool, std::string>>
    map_bool_args;
typedef std::tuple<std::string, std::vector<std::string>, std::string>
		tuple_positional_args;

/// Interface to get argument options
class ICliArgsGetter {
public:
	virtual ~ICliArgsGetter() {}
	virtual int init(std::string_view program_name,
			std::string_view program_description) = 0;
	virtual void add_options(const map_string_args &string_args) = 0;
	virtual void add_options(const map_int_args &int_args) = 0;
	virtual void add_options(const map_bool_args &bool_args) = 0;
  virtual int parse_options(int argc, char **argv) = 0;
  virtual int get_arg(std::string_view name, int def) const = 0;
  virtual std::string get_arg(std::string_view name,
                              const std::string &def) const = 0;
  virtual bool get_arg(std::string_view name, bool def) const = 0;
};

typedef struct _Options {
  /// Option name, {option value, option help text}
  map_string_args str_args;
  map_int_args int_args;
  map_bool_args bool_args;

  /// Storage for poitional args
  tuple_positional_args pos_arg;

	std::string_view description = "gnvim - GUI for neovim";
  std::string_view positional_help = "[optional args]";
  /// Contains information to show when using -h
  std::string help;
  // TODO- Create a version string

  _Options():
    str_args({{"n,nvim", {"nvim", "nvim executable path"}}}),
    int_args({{"t,timeout",
                 {15, "Error if nvim does not responde after count seconds"}}}),
		bool_args({{"m,maximized", {false, "Maximize the window on startup"}},
				{"h,help", {false, "Print this help"}},
				{"v,version", {false, "Print version information"}}}),
    pos_arg({"positional", {}, "List of files to open"})
		{}
} Options;

/// Local interface to get options
/// Provides abstraction from external libraries
class CliArgs : public ICliArgsGetter {
public:
  CliArgs() {}
  virtual ~CliArgs() {}

	int init(std::string_view program_name,
			std::string_view program_description) override;
	void add_options(const map_string_args &string_args) override;
	void add_options(const map_int_args &int_args) override;
	void add_options(const map_bool_args &bool_args) override;
  int get_arg(std::string_view name, int def) const override;
  std::string get_arg(std::string_view name,
                              const std::string &def) const override;
  bool get_arg(std::string_view name, bool def) const override;
  const std::vector<std::string> &get_positional_arg();
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
