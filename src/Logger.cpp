/*
 * Copyright (c) 2020, Мира Странная <miraityan2004@gmail.com>
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

#include "Logger.hpp"

#include <iostream>
#include <algorithm>
#include <ctime>

#include "utils.hpp"

namespace mazemaze {

Logger::Logger() = default;
Logger::~Logger() = default;

void
Logger::log(const std::string& level, const std::string& message) {
    char indent[] = "        ";
    indent[7 - level.size()] = '\0';

    std::string fullMessage = format("[%d] [%s]%s%s",
                                     std::time(nullptr),
                                     level.c_str(),
                                     indent,
                                     message.c_str());

    std::cout << fullMessage << std::endl;

    messages.emplace_back(fullMessage);
}

}
