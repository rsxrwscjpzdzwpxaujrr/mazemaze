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

#pragma once

#include <string>
#include <chrono>
#include <mutex>
#include <functional>
#include <vector>

#define DEBUG_LEVEL  "DEBUG"
#define STATUS_LEVEL "STATUS"
#define WARN_LEVEL   "WARN"
#define ERROR_LEVEL  "ERROR"

namespace mazemaze {

using std::chrono::system_clock;

class Logger {
public:
    enum Level {
        DEBUG  = 0,
        STATUS = 1,
        WARN   = 2,
        ERR    = 3
    };

    class Message {
    public:
        Message(Level level, const std::string& message);

        const system_clock::time_point time;
        const Level level;
        const std::string message;

        std::string to_string() const;
    };

    Logger(Logger const&) = delete;
    void operator= (Logger const&) = delete;

    static Logger& inst() {
        static Logger instance;
        return instance;
    }

    void log_debug (const std::string& message);
    void log_status(const std::string& message);
    void log_warn  (const std::string& message);
    void log_error (const std::string& message);

    int  add_message_listener(std::function<void(Message& message)> message_listener);

    void remove_message_listener(int id);
    std::vector<Message>& messages();

    system_clock::time_point init_time();
private:
    Logger();
    ~Logger();

    void log(Level level, const std::string& message);

    const system_clock::time_point m_init_time;
    std::vector<Message> m_messages;
    std::vector<std::function<void(Message&)>> message_listeners;
    std::mutex mutex;
};

inline void
Logger::log_debug(const std::string& message) {
#ifndef _NDEBUG
    log(DEBUG, message);
#endif
}

inline void
Logger::log_status(const std::string& message) {
    log(STATUS, message);
}

inline void
Logger::log_warn(const std::string& message) {
    log(WARN, message);
}

inline void
Logger::log_error(const std::string& message) {
    log(ERR, message);
}

}
