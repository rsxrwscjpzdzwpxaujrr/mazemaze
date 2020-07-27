/*
 * Copyright (c) 2019, Мира Странная <rsxrwscjpzdzwpxaujrr@yahoo.com>
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

#include "utils.hpp"

#include <cstdarg>
#include <vector>

namespace mazemaze {

static std::string
_fmt(const char *fmt, va_list args) {
    std::vector<char> v(64);

    while (true) {
        va_list args2;
        va_copy(args2, args);
        int res = vsnprintf(v.data(), v.size(), fmt, args2);

        if ((res >= 0) && (res < static_cast<int>(v.size()))) {
            va_end(args);
            va_end(args2);
            return std::string(v.data());
        }

        size_t size;

        if (res < 0)
            size = v.size() * 2;
        else
            size = static_cast<size_t>(res) + 1;

        v.clear();
        v.resize(size);
        va_end(args2);
    }
}

std::string
fmt(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);

    return _fmt(fmt, args);

    va_end(args);
}

sf::String
pgtx(const char* msgctxt, const char* msgid) {
    std::string tmp = pgettext_expr(msgctxt, msgid);

    return sf::String::fromUtf8(tmp.begin(), tmp.end());
}

sf::String
pgtxf(const char* msgctxt, const char* msgid, ...) {
    std::string tmp = pgettext_expr(msgctxt, msgid);

    va_list args;
    va_start(args, msgid);

    tmp = _fmt(tmp.c_str(), args);

    va_end(args);

    return sf::String::fromUtf8(tmp.begin(), tmp.end());
}

sf::String
npgtxf(const char* msgctxt, const char* msgid, const char* msgidPlural, int n) {
    std::string tmp = fmt(npgettext_expr(msgctxt, msgid, msgidPlural, n), n);

    return sf::String::fromUtf8(tmp.begin(), tmp.end());
}

}
