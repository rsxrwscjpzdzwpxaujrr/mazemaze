/*
 * Copyright (c) 2018-2021, Мира Странная <rsxrwscjpzdzwpxaujrr@yahoo.com>
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
    for (unsigned int i = 0; i < SIZE; i++)
        for (unsigned int j = 0; j < SIZE; j++)
            opened[j][i] = false;
}

Chunk::~Chunk() = default;

bool
Chunk::get_opened(unsigned int x, unsigned int y) {
    return opened[x][y];
}

void
Chunk::set_opened(unsigned int x, unsigned int y, bool opened) {
    Chunk::opened[x][y] = opened;
}

}
