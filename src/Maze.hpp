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

#pragma once

#include <random>
#include <stack>

#include <SFML/System/Vector2.hpp>

namespace mazemaze {

class Chunk;

class Maze {
public:
    struct Generator {
        Generator(short x, short y);
        Generator(short x, short y, int side);

        unsigned short x;
        unsigned short y;
        unsigned char tried;
    };


    explicit Maze(int width, int height);
    ~Maze();

    bool generate(unsigned int seed);
    void cancelGeneration();
    float getGenerationProgress() const;

    void setExitX(int exitX);
    void setExitY(int exitY);
    void setStartX(int startX);
    void setStartY(int startY);

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

    void initChunks();

private:
    void setOpened(int x, int y, bool opened);
    void genExit(std::mt19937& random);
    void genStart(std::mt19937& random);
    bool genStep(std::stack<Generator>& generator, Generator* currentGenerator, int side);

    int anglesOpened;
    bool needCancel;

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
