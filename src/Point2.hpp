/*
 * Copyright (c) 2021, Мира Странная <rsxrwscjpzdzwpxaujrr@yahoo.com>
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

namespace mazemaze {

template<typename T>
class Point2 {
public:
    explicit Point2(T x, T y);
    Point2();

    void set(Point2<T> point);

    bool operator==(Point2<T> point);

    T x;
    T y;
};

template class Point2<float>;
template class Point2<int>;

typedef Point2<float> Point2f;
typedef Point2<int>   Point2i;

}
