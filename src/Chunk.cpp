/*
 * Copyright (c) 2018-2019, Мира Странная <miraityan2004@gmail.com>
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

#include "Chunk.hpp"

namespace mazemaze {

Chunk::Chunk() {
    for (uint i = 0; i < SIZE; i++)
        for (uint j = 0; j < SIZE; j++)
            opened[j][i] = false;
}

bool
Chunk::getOpened(uint x, uint y) {
    if (inBound(x) && inBound(y))
        return opened[x][y];
    else
        return true;
}

void
Chunk::setOpened(uint x, uint y, bool opened) {
    if (inBound(x) && inBound(y))
        Chunk::opened[x][y] = opened;
}

bool
Chunk::inBound(uint i) {
    return i < SIZE;
}

}
