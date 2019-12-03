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

#include <random>

#include <SFML/System/Vector2.hpp>

namespace mazemaze {

class Chunk;

class Maze {
public:
    explicit Maze(int width, int height);
    ~Maze();

    void generate(unsigned int seed);

    bool getOpened(int x, int y);
    int getWidth() const;
    int getHeight() const;
    unsigned int getSeed() const;
    int getExitX() const;
    int getExitY() const;
    int getStartX() const;
    int getStartY() const;
    Chunk* getChunks() const;
    int getChunksCount() const;
    unsigned int getChunksX() const;
    unsigned int getChunksY() const;

    void setSeed(unsigned int seed);

private:
    void setOpened(int x, int y, bool opened);
    void genExit(std::mt19937& random);
    void genStart(std::mt19937& random);
    bool genStep(sf::Vector2i& generator, bool tried[], int side);

    int exitX;
    int exitY;
    int startX;
    int startY;
    int width;
    int height;
    unsigned int seed;
    unsigned int chunksX;
    unsigned int chunksY;
    Chunk* chunks;
};

}
