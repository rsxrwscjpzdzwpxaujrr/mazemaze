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

#include "Maze.hpp"

#include <stack>
#include <random>
#include <stdexcept>

#include <SFML/System/Vector2.hpp>

#include "Chunk.hpp"

namespace mazemaze {

Maze::Maze(int width, int height) {
    if (width < 1 || height < 1)
        throw std::invalid_argument("Width and height must be 1 or bigger");

    Maze::width  = width  * 2 + 1;
    Maze::height = height * 2 + 1;

    unsigned int uWidth  = static_cast<unsigned int>(Maze::width);
    unsigned int uHeight = static_cast<unsigned int>(Maze::height);

    chunksX = uWidth  / Chunk::SIZE;
    chunksY = uHeight / Chunk::SIZE;

    if (uWidth % Chunk::SIZE != 0)
        chunksX++;

    if (uHeight % Chunk::SIZE != 0)
        chunksY++;

    chunks = new Chunk[chunksX * chunksY] {Chunk()};
}

Maze::~Maze() {
    delete [] chunks;
}

bool
Maze::genStep(sf::Vector2i* generator, bool tried[], int side) {
    if (tried[side])
        return false;

    int x = 0;
    int y = 0;

    if (side >= 2)
        y = side % 2 * 2 - 1;
    else
        x = side % 2 * 2 - 1;

    if (getOpened(generator->x + x * 2, generator->y + y * 2)) {
        tried[side] = true;

        return false;
    } else {
        setOpened(generator->x + x,     generator->y + y,     true);
        setOpened(generator->x + x * 2, generator->y + y * 2, true);

        for (int i = 0; i < 4; i++)
            tried[i] = false;

        generator->x += x * 2;
        generator->y += y * 2;

        return true;
    }
}

unsigned int
Maze::getSeed() const {
    return seed;
}

void
Maze::generate(unsigned int seed) {
    std::stack<sf::Vector2i> generators;
    sf::Vector2i currentGenerator(1, 1);

    setOpened(currentGenerator.x, currentGenerator.x, true);
    generators.emplace(currentGenerator);

    std::mt19937 randGen(seed);
    std::uniform_int_distribution<> sideDistrib(0, 3);

    bool done = false;
    bool tried[4] = {false};

    while (!done) {
        int side = sideDistrib(randGen);

        if (genStep(&currentGenerator, tried, side))
            generators.emplace(currentGenerator);

        bool goBack = true;

        for (int i = 0; i < 4; i++)
            goBack &= tried[i];

        if (goBack) {
            generators.pop();

            for (int i = 0; i < 4; i++)
                tried[i] = false;

            if (generators.empty())
                done = true;
            else
                currentGenerator = generators.top();
        }
    }

    setOpened(getExitX(), getExitY(), true);
    Maze::seed = seed;
}

bool
Maze::getOpened(int x, int y) {
    if (x < 0 || x >= width || y < 0 || y >= height)
        return true;

    unsigned int ux = static_cast<unsigned int>(x);
    unsigned int uy = static_cast<unsigned int>(y);

    return chunks[((uy / Chunk::SIZE) * chunksY) + (ux / Chunk::SIZE)]
           .getOpened(ux % Chunk::SIZE, uy % Chunk::SIZE);
}

void
Maze::setOpened(int x, int y, bool opened) {
    if (x < 0 || x >= width || y < 0 || y >= height)
        return;

    unsigned int ux = static_cast<unsigned int>(x);
    unsigned int uy = static_cast<unsigned int>(y);

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

Chunk*
Maze::getChunks() const {
    return chunks;
}

int
Maze::getChunksCount() const {
    return chunksX * chunksY;
}

unsigned int
Maze::getChunksX() const {
    return chunksX;
}

unsigned int
Maze::getChunksY() const {
    return chunksY;
}

void
Maze::setSeed(unsigned int seed) {
    Maze::seed = seed;
}

}
