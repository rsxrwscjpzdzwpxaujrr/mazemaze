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

    std::string tmp = _fmt(fmt, args);

    va_end(args);

    return tmp;
}

void
side_to_coords(int side, int& x, int& y) {
    static const int sidesX[4] {-1, 1, 0, 0};
    static const int sidesY[4] {0, 0, -1, 1};

    x = sidesX[side];
    y = sidesY[side];
}

int opposite_side(int side) {
    static const int oppSide[4] {1, 0, 3, 2};

    return oppSide[side];
}

sf::String
pgtx(const char* text, const char* id) {
    std::string tmp = pgettext_expr(text, id);

    return sf::String::fromUtf8(tmp.begin(), tmp.end());
}

sf::String
pgtxf(const char* text, const char* id, ...) {
    std::string tmp = pgettext_expr(text, id);

    va_list args;
    va_start(args, id);

    tmp = _fmt(tmp.c_str(), args);

    va_end(args);

    return sf::String::fromUtf8(tmp.begin(), tmp.end());
}

sf::String
npgtxf(const char* text, const char* id, const char* plural, int n) {
    std::string tmp = fmt(npgettext_expr(text, id, plural, n), n);

    return sf::String::fromUtf8(tmp.begin(), tmp.end());
}

}
