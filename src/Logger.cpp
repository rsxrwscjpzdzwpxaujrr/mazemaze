/*
 * Copyright (c) 2020-2021, Мира Странная <rsxrwscjpzdzwpxaujrr@yahoo.com>
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

Logger::Logger() : m_init_time(system_clock::now().time_since_epoch()) {
};

Logger::~Logger() = default;

void
Logger::log(Level level, const std::string& message) {
    std::lock_guard<std::mutex> lock(mutex);

    m_messages.emplace_back(Message(level, message));

    std::ostream* stream;

    if (level < WARN)
        stream = &std::cout;
    else
        stream = &std::cerr;

    *stream << m_messages.back().to_string() << std::endl;

    for (auto& message_listener: message_listeners)
        message_listener(m_messages.back());
}

int
Logger::add_message_listener(std::function<void(Message&)> message_listener) {
    message_listeners.emplace_back(message_listener);

    return message_listeners.size() - 1;
}

void
Logger::remove_message_listener(int id) {
    message_listeners.erase(message_listeners.begin() + id);
}

std::vector<Logger::Message>&
Logger::messages() {
    return Logger::m_messages;
}

system_clock::time_point
Logger::init_time() {
    return m_init_time;
}

Logger::Message::Message(Logger::Level level, const std::string& message) :
        time(system_clock::now()),
        level(level),
        message(message) {
}

std::string
Logger::Message::to_string() const {
    using namespace std::chrono;

    std::string level_text = "";

    switch (level) {
    case DEBUG:
        level_text = DEBUG_LEVEL;
        break;

    case STATUS:
        level_text = STATUS_LEVEL;
        break;

    case WARN:
        level_text = WARN_LEVEL;
        break;

    case ERR:
        level_text = ERROR_LEVEL;
        break;
    }

    auto since_init = time - Logger::inst().m_init_time;

    std::string time_string = fmt(
        "%d.%03d",
        duration_cast<seconds>(since_init).count(),
        duration_cast<milliseconds>(since_init).count() % 1000
    );

    int time_chars = 5;

    char indent[]      = "        ";
    char time_indent[] = "     ";

    indent[7 - level_text.size()] = '\0';

    if (time_string.size() < time_chars + 4) {
        time_indent[time_chars - (time_string.size() - 4)] = '\0';
    } else {
        time_indent[0] = '\0';
    }

    return fmt(
        "[%s%s] [%s]%s%s",
        time_indent,
        time_string.c_str(),
        level_text.c_str(),
        indent,
        message.c_str()
    );
}

}
