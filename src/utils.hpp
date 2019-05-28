/*
 * Copyright (c) 2019, Мира Странная <miraityan2004@gmail.com>
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

std::string format(const char *fmt, ...);
sf::String pgtx(const char* msgctxt, const char* msgid);
sf::String npgtxf(const char* msgctxt, const char* msgid, const char* msgidPlural, int n);

}
