/*
 * Copyright (c) 2019-2021, Мира Странная <rsxrwscjpzdzwpxaujrr@yahoo.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <gettext.h>
#include <string>

#include <SFML/System.hpp>
#include <SFML/System/Utf.hpp>

namespace mazemaze {

std::string fmt(const char *fmt, ...);
void side_to_coords(int side, int& x, int& y);
int opposite_side(int side);

sf::String pgtx  (const char* text, const char* id);
sf::String pgtxf (const char* text, const char* id, ...);
sf::String npgtxf(const char* text, const char* id, const char* plural, int n);

}
