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

#pragma once

#include <vector>

#include <SFML/System/Vector2.hpp>

namespace mazemaze {

class Chunk;

typedef unsigned int uint;

class Maze {
public:
    explicit Maze(int width, int height);
    ~Maze();

    void generate(uint seed);
    bool getOpened(int x, int y);
    int getWidth() const;
    int getHeight() const;
    int getExitX() const;
    int getExitY() const;

private:
    void setOpened(int x, int y, bool opened);
    bool genStep(sf::Vector2i* generator, bool tried[], int side);

    int width;
    int height;
    uint seed;
    uint chunksX;
    uint chunksY;
    std::vector<Chunk> chunks;
};

}
