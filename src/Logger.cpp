/*
 * Copyright (c) 2020, Мира Странная <rsxrwscjpzdzwpxaujrr@yahoo.com>
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
#include <chrono>

#include "utils.hpp"

namespace mazemaze {

Logger::Logger() = default;
Logger::~Logger() = default;

void
Logger::log(Level level, const std::string& message) {
    using namespace std::chrono;

    std::string levelText = "";

    switch (level) {
    case DEBUG:
        levelText = DEBUG_LEVEL;
        break;

    case STATUS:
        levelText = STATUS_LEVEL;
        break;

    case WARN:
        levelText = WARN_LEVEL;
        break;

    case ERROR:
        levelText = ERROR_LEVEL;
        break;
    }

    char indent[] = "        ";
    indent[7 - levelText.size()] = '\0';

    auto now = system_clock::now().time_since_epoch();

    std::string fullMessage = format("[%d.%03d] [%s]%s%s",
                                     duration_cast<seconds>(now).count(),
                                     duration_cast<milliseconds>(now).count() % 1000,
                                     level.c_str(),
                                     indent,
                                     message.c_str());

    std::cout << fullMessage << std::endl;

    messages.emplace_back(fullMessage);
}

}
