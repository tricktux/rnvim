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
#include <unordered_map>

class ICliArgsGetter {
protected:
  ICliArgsGetter() {}
  virtual ~ICliArgsGetter() {}

public:
  virtual int get_arg(const char *name, int def) = 0;
  virtual std::string get_arg(const char *name, const std::string &def) = 0;
  virtual bool get_arg(const char *name, bool def) = 0;
};

class CliArgs : public ICliArgsGetter {
  std::unordered_map<std::string, std::string> str_args;
  std::unordered_map<std::string, int> int_args;
  std::unordered_map<std::string, bool> bool_args;

public:
  CliArgs() {
    str_args = {{"nvim", "nvim"}};

    bool_args = {{"maximized", false}};

    int_args = {{"timeout", 1000}};
  }

  virtual ~CliArgs() {}

  void init(ICliArgsGetter &getter);
  virtual int get_arg(const char *name, int def) final;
  virtual std::string get_arg(const char *name, const std::string &def) final;
  virtual bool get_arg(const char *name, bool def) final;
};

class CxxOptsArgs : public ICliArgsGetter {

public:
  CxxOptsArgs() {}
  virtual ~CxxOptsArgs() {}

  void init(int argc, char **argv);
  virtual int get_arg(const char *name, int def) final;
  virtual std::string get_arg(const char *name, const std::string &def) final;
  virtual bool get_arg(const char *name, bool def) final;
};
