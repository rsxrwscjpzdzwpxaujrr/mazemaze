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

#include <stack>
#include <random>
#include <iostream>
#include <cmath>

#include "Chunk.hpp"

#include "Maze.hpp"

namespace mazemaze {

Maze::Maze(int width, int height) {
    if (width < 1 || height < 1)
        throw std::invalid_argument("Width and height must be 1 or bigger");

    Maze::width = width * 2 + 1;
    Maze::height = height * 2 + 1;

    uint uWidth = static_cast<uint>(Maze::width);
    uint uHeight = static_cast<uint>(Maze::height);

    chunksX = uWidth / Chunk::SIZE;
    chunksY = uHeight / Chunk::SIZE;

    if (uWidth % Chunk::SIZE != 0)
        chunksX++;

    if (uHeight % Chunk::SIZE != 0)
        chunksY++;

    for (uint i = 0; i < chunksX * chunksY; i++)
        chunks.push_back(Chunk());
}

Maze::~Maze() {
    chunks.clear();
}

void
Maze::generate(uint seed) {
    struct point {
        point(int x, int y) {
            point::x = x;
            point::y = y;
        }

        int x;
        int y;
    };

    std::stack<point> generators;
    point currentGenerator = point(1, 1);
    bool done = false;
    bool triedR = false;
    bool triedL = false;
    bool triedU = false;
    bool triedB = false;

    Maze::seed = seed;
    setOpened(currentGenerator.x, currentGenerator.x, true);

    generators.push(currentGenerator);

    std::mt19937 randGen(seed);
    std::uniform_int_distribution<> sideDistrib(0, 3);

    int side;

    while (!done) {
        if (triedR && triedL && triedU && triedB) {
            generators.pop();

            triedR = false;
            triedL = false;
            triedU = false;
            triedB = false;

            if (generators.empty())
                done = true;
            else
                currentGenerator = generators.top();
        }

        side = sideDistrib(randGen);

        switch (side) {
            case 0:
                if (!triedR) {
                    if (getOpened(currentGenerator.x + 2, currentGenerator.y)) {
                        triedR = true;
                        continue;
                    }

                    setOpened(currentGenerator.x + 1, currentGenerator.y, true);
                    setOpened(currentGenerator.x + 2, currentGenerator.y, true);

                    triedR = false;
                    triedL = false;
                    triedU = false;
                    triedB = false;

                    currentGenerator.x += 2;
                    generators.push(currentGenerator);
                }

                break;

            case 1:
                if (!triedL) {
                    if (getOpened(currentGenerator.x - 2, currentGenerator.y)) {
                        triedL = true;
                        continue;
                    }

                    setOpened(currentGenerator.x - 1, currentGenerator.y, true);
                    setOpened(currentGenerator.x - 2, currentGenerator.y, true);

                    triedR = false;
                    triedL = false;
                    triedU = false;
                    triedB = false;

                    currentGenerator.x -= 2;
                    generators.push(currentGenerator);
                }

                break;

            case 2:
                if (!triedU) {
                    if (getOpened(currentGenerator.x, currentGenerator.y + 2)) {
                        triedU = true;
                        continue;
                    }

                    setOpened(currentGenerator.x, currentGenerator.y + 1, true);
                    setOpened(currentGenerator.x, currentGenerator.y + 2, true);

                    triedR = false;
                    triedL = false;
                    triedU = false;
                    triedB = false;

                    currentGenerator.y += 2;
                    generators.push(currentGenerator);
                }

                break;

            case 3:
                if (!triedB) {
                    if (getOpened(currentGenerator.x, currentGenerator.y - 2)) {
                        triedB = true;
                        continue;
                    }

                    setOpened(currentGenerator.x, currentGenerator.y - 1, true);
                    setOpened(currentGenerator.x, currentGenerator.y - 2, true);

                    triedR = false;
                    triedL = false;
                    triedU = false;
                    triedB = false;

                    currentGenerator.y -= 2;
                    generators.push(currentGenerator);
                }

                break;
        }
    }

    //setOpened(0, 1, true);
    setOpened(getExitX(), getExitY(), true);
}

bool
Maze::getOpened(int x, int y) {
    if (x < 0 || x >= width || y < 0 || y >= height)
        return true;

    uint ux = static_cast<uint>(x);
    uint uy = static_cast<uint>(y);

    return chunks[((uy / Chunk::SIZE) * chunksY) + (ux / Chunk::SIZE)]
           .getOpened(ux % Chunk::SIZE, uy % Chunk::SIZE);
}

void
Maze::setOpened(int x, int y, bool opened) {
    if (x < 0 || x >= width || y < 0 || y >= height)
        return;

    uint ux = static_cast<uint>(x);
    uint uy = static_cast<uint>(y);

    chunks[((uy / Chunk::SIZE) * chunksY) + (ux / Chunk::SIZE)]
            .setOpened(ux % Chunk::SIZE, uy % Chunk::SIZE, opened);
}

int
Maze::getWidth() const {
    return width;
}

int
Maze::getHeight() const {
    return height;
}

int
Maze::getExitX() const {
    return width - 1;
}

int
Maze::getExitY() const {
    return height - 2;
}

}
