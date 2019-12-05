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
#include <stdexcept>
#include <algorithm>

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
Maze::genStep(sf::Vector2i& generator, bool tried[], int side) {
    if (tried[side])
        return false;

    int x = 0;
    int y = 0;

    if (side >= 2)
        y = side % 2 * 2 - 1;
    else
        x = side % 2 * 2 - 1;

    int newx = generator.x + x * 2;
    int newy = generator.y + y * 2;

    bool inbound = newx >= 0 && newx < width && newy >= 0 && newy < height;

    if (!inbound || getOpened(newx, newy)) {
        tried[side] = true;

        return false;
    } else {
        setOpened(generator.x + x, generator.y + y, true);
        setOpened(newx, newy, true);

        for (int i = 0; i < 4; i++)
            tried[i] = false;

        generator.x = newx;
        generator.y = newy;

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

    setOpened(currentGenerator.x, currentGenerator.y, true);
    generators.emplace(currentGenerator);

    std::mt19937 randGen(seed);
    std::uniform_int_distribution<> sideDistrib(0, 3);

    genStart(randGen);
    genExit(randGen);

    bool done = false;
    bool tried[4] = {false};

    while (!done) {
        int side = sideDistrib(randGen);

        if (genStep(currentGenerator, tried, side))
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

    return chunks[((uy / Chunk::SIZE) * chunksX) + (ux / Chunk::SIZE)]
           .getOpened(ux % Chunk::SIZE, uy % Chunk::SIZE);
}

void
Maze::setOpened(int x, int y, bool opened) {
    unsigned int ux = static_cast<unsigned int>(x);
    unsigned int uy = static_cast<unsigned int>(y);

    chunks[((uy / Chunk::SIZE) * chunksX) + (ux / Chunk::SIZE)]
            .setOpened(ux % Chunk::SIZE, uy % Chunk::SIZE, opened);
}

void
Maze::genExit(std::mt19937& random) {
    std::uniform_int_distribution<> distrib(0, 8);
    bool direction;

    do {
        int number = distrib(random);
        int angle = number / 2;
        direction = number % 2 == 1;

        exitX = std::max(std::min(angle % 2 * width,  width  - 2), 1);
        exitY = std::max(std::min(angle / 2 * height, height - 2), 1);
    } while (exitX == startX && exitY == startY && !(width <= 3 && height <= 3));

    int* directedCoord;

    if (direction)
        directedCoord = &exitX;
    else
        directedCoord = &exitY;

    if (*directedCoord == 1)
        (*directedCoord)--;
    else
        (*directedCoord)++;
}

void
Maze::genStart(std::mt19937& random) {
    std::uniform_int_distribution<> distrib(0, 4);

    int number = distrib(random);
    int angle = number / 2;

    startX = std::max(std::min(angle % 2 * width,  width  - 2), 1);
    startY = std::max(std::min(angle / 2 * height, height - 2), 1);
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
    return exitX;
}

int
Maze::getExitY() const {
    return exitY;
}

int
Maze::getStartX() const {
    return startX;
}

int
Maze::getStartY() const {
    return startY;
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
