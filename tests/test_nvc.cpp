/// @file test_nvc.cpp
/// @brief Test libnvc with the purpose of comparing functionality and 
/// performance
/// @author Reinaldo Molina
/// @version  0.0
/// @date Jan 02 2020
// Copyright © 2020 Reinaldo Molina

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


#include "libnvc.hpp"
#include <gtest/gtest.h>

TEST(nvc, readme) {

	std::string buf{"yixx"};
	libnvc::reproc_device reproc_dev;
	reproc_dev.spawn();

	libnvc::api_client client(&reproc_dev);
	client.nvim_ui_attach(100, 80, {{"rgb", true}});
	client.nvim_input("$i123<CR>123<ESC>");
	client.nvim_buf_set_name(1, ":D");
	client.nvim_buf_get_name(1);
	reproc_dev.kill();
}
