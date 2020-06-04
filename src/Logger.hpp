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

#pragma once

#include <deque>
#include <string>

#define DEBUG_LEVEL  "DEBUG"
#define STATUS_LEVEL "STATUS"
#define WARN_LEVEL   "WARN"
#define ERROR_LEVEL  "ERROR"

namespace mazemaze {

class Logger {
public:
    Logger(Logger const&) = delete;
    void operator= (Logger const&) = delete;

    static Logger& inst() {
        static Logger instance;
        return instance;
    }

    void log_debug(const std::string& message);
    void log_status(const std::string& message);
    void log_warn(const std::string& message);
    void log_error(const std::string& message);

private:
    Logger();
    ~Logger();

    void log(const std::string& level, const std::string& message);

    std::deque<std::string> messages;
};

inline void
Logger::log_debug(const std::string& message) {
#ifndef _NDEBUG
    log(DEBUG_LEVEL, message);
#endif
}

inline void
Logger::log_status(const std::string& message) {
    log(STATUS_LEVEL, message);
}

inline void
Logger::log_warn(const std::string& message) {
    log(WARN_LEVEL, message);
}

inline void
Logger::log_error(const std::string& message) {
    log(ERROR_LEVEL, message);
}

}
