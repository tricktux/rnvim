/// @file process_handler.cpp
/// @brief Responsible for handling the neovim process
/// @author Reinaldo Molina
/// @version  0.0
/// @date Apr 15 2019
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

#pragma once

#include <reproc++/reproc.hpp>
#include <reproc++/sink.hpp>

#include <future>
#include <iostream>
#include <vector>
#include <string>

class IProcessHandler {
protected:
	bool is_running;
	unsigned int timeout;
public:
	virtual int start(const std::vector<std::string> &cmd, unsigned int timeout);
	virtual int stop();
};

class ProcessHandler : public IProcessHandler {
	reproc::process nvim;
public:
	ProcessHandler() : is_running(false), timeout(0) {}

	virtual int start(const std::vector<std::string> &cmd, unsigned int timeout);
	virtual int stop();
};
