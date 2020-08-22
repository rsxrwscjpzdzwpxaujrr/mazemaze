/*
 * Copyright (c) 2018-2020, Мира Странная <rsxrwscjpzdzwpxaujrr@yahoo.com>
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

#include <stdexcept>

#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>

#include "Chunk.hpp"
#include "Logger.hpp"
#include "utils.hpp"

namespace mazemaze {

Maze::Maze(int width, int height) :
        chunks(nullptr) {
    if (width < 1 || height < 1)
        throw std::invalid_argument("Width and height must be 1 or bigger");

    Maze::width  = width  * 2 + 1;
    Maze::height = height * 2 + 1;

    chunksX = Maze::width  / Chunk::SIZE;
    chunksY = Maze::height / Chunk::SIZE;

    if (width % Chunk::SIZE != 0)
        chunksX++;

    if (height % Chunk::SIZE != 0)
        chunksY++;
}

Maze::~Maze() {
    delete [] chunks;
}

bool
Maze::genStep(std::stack<Generator>& generators, Generator* generator, int side) {
    int x;
    int y;

    if (side < 2) {
        x = side % 2 * 2 - 1;
        y = 0;
    } else {
        x = 0;
        y = side % 2 * 2 - 1;
    }

    int newx = generator->x + x * 2;
    int newy = generator->y + y * 2;

    bool inbound = newx >= 0 && newx < width && newy >= 0 && newy < height;

    if (!inbound || getOpened(newx, newy)) {
        generator->tried |= 1 << side;

        return false;
    } else {
        setOpened(generator->x + x, generator->y + y, true);
        setOpened(newx, newy, true);
        anglesOpened++;

        generators.emplace(Generator(newx, newy, side));

        return true;
    }
}

unsigned int
Maze::getSeed() const {
    return seed;
}

bool
Maze::generate(unsigned int seed) {
    Logger::inst().log_debug(fmt("Maze generation started. Size is %dx%d",
                                 (width - 1) / 2,
                                 (height - 1) / 2));
    initChunks();

    anglesOpened = 0;
    needCancel = false;

    std::stack<Generator> generators;
    generators.emplace(Generator(1, 1));
    Generator* currentGenerator = &generators.top();

    setOpened(currentGenerator->x, currentGenerator->y, true);

    anglesOpened++;

    std::mt19937 randGen(seed);
    std::uniform_int_distribution<> sideDistrib(0, 3);

    genStart(randGen);
    genExit(randGen);

    bool done = false;

    sf::Clock clock;
    sf::Time lastTime = clock.getElapsedTime();

    while (!done) {
        bool goBack = currentGenerator->tried == 0xf;

        if (goBack) {
            generators.pop();

            if (generators.empty()) {
                done = true;
                break;
            }

            currentGenerator = &generators.top();
        } else {
            int side = sideDistrib(randGen);

            while (currentGenerator->tried & (1 << side)) {
                side = sideDistrib(randGen);
            }

            if (genStep(generators, currentGenerator, side)) {
                currentGenerator = &generators.top();
            }
        }

        if (clock.getElapsedTime() - lastTime >= sf::milliseconds(1000)) {
            Logger::inst().log_debug(fmt("Progress: %.1f%%", getGenerationProgress() * 100.0f));
            lastTime = clock.getElapsedTime();
        }

        if (needCancel) {
            needCancel = false;
            anglesOpened = 0;

            Logger::inst().log_warn("Maze generation canceled.");
            return false;
        }
    }

    setOpened(getExitX(), getExitY(), true);

    Maze::seed = seed;

    Logger::inst().log_status(fmt("Maze generation completed. "
                                  "It took %.2f sec",
                                  clock.getElapsedTime().asMilliseconds() / 1000.0f));
    return true;
}

void
Maze::cancelGeneration() {
    needCancel = true;
}

float
Maze::getGenerationProgress() const {
    return anglesOpened / static_cast<float>(((width - 1) / 2) * ((height - 1) / 2));
}

void
Maze::setExitX(int exitX) {
    Maze::exitX = exitX;
}

void
Maze::setExitY(int exitY) {
    Maze::exitY = exitY;
}

void
Maze::setStartX(int startX) {
    Maze::startX = startX;
}

void
Maze::setStartY(int startY) {
    Maze::startY = startY;
}


bool
Maze::getOpened(int x, int y) {
    if (!chunks || x < 0 || x >= width || y < 0 || y >= height)
        return true;

    unsigned int ux = static_cast<unsigned int>(x);
    unsigned int uy = static_cast<unsigned int>(y);

    return chunks[((uy / Chunk::SIZE) * chunksX) + (ux / Chunk::SIZE)]
           .getOpened(ux % Chunk::SIZE, uy % Chunk::SIZE);
}

void
Maze::setOpened(int x, int y, bool opened) {
    if (!chunks)
        return;

    unsigned int ux = static_cast<unsigned int>(x);
    unsigned int uy = static_cast<unsigned int>(y);

    chunks[((uy / Chunk::SIZE) * chunksX) + (ux / Chunk::SIZE)]
            .setOpened(ux % Chunk::SIZE, uy % Chunk::SIZE, opened);
}

void
Maze::genExit(std::mt19937& random) {
    std::uniform_int_distribution<> distrib(0, 3);
    std::uniform_int_distribution<> boolDistrib(0, 1);

    do {
        int angle = distrib(random);

        exitX = ((angle % 2) * (width - 3)) + 1;
        exitY = ((angle / 2) * (height - 3)) + 1;
    } while (exitX == startX && exitY == startY && !(width <= 3 && height <= 3));

    bool direction = boolDistrib(random);

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
    std::uniform_int_distribution<> distrib(0, 3);

    int angle = distrib(random);

    startX = ((angle % 2) * (width - 3)) + 1;
    startY = ((angle / 2) * (height - 3)) + 1;
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

void
Maze::initChunks() {
    if (chunks)
        delete [] chunks;

    chunks = new Chunk[getChunksCount()] {Chunk()};
}

Maze::Generator::Generator(short x, short y) : x(x), y(y), tried(0) {}

Maze::Generator::Generator(short x, short y, int side) : x(x), y(y) {
    static const int oppside[4] {1, 0, 3, 2};

    tried = 1 << oppside[side];
}

}
