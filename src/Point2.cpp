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

#include "Point2.hpp"

namespace mazemaze {

template<typename T>
Point2<T>::Point2(T x, T y) : x(x), y(y) {
}

template<typename T>
Point2<T>::Point2() : x(0), y(0) {
}

template<typename T>
void
Point2<T>::set(Point2<T> point) {
    x = point.x;
    y = point.y;
}

template<typename T>
bool
Point2<T>::operator==(Point2<T> point) {
    return x == point.x && y == point.y;
}

}
