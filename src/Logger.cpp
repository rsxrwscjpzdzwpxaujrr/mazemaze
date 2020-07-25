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
    Message message_obj(level, message);

    std::cout << message_obj.to_string() << std::endl;

    messages.emplace_back(message_obj);
}

Logger::Message::Message(Logger::Level level, const std::string& message) :
        time(std::chrono::system_clock::now()),
        level(level),
        message(message) {
}

std::string
Logger::Message::to_string() {
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

    case ERR:
        levelText = ERROR_LEVEL;
        break;
    }

    char indent[] = "        ";
    indent[7 - levelText.size()] = '\0';

    auto since_epoch = time.time_since_epoch();

    return format("[%d.%03d] [%s]%s%s",
           duration_cast<seconds>(since_epoch).count(),
           duration_cast<milliseconds>(since_epoch).count() % 1000,
           levelText.c_str(),
           indent,
           message.c_str());
}

}
