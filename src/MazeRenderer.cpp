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

#include "MazeRenderer.hpp"

#include <SFML/OpenGL.hpp>

#include "Chunk.hpp"

namespace mazemaze {

MazeRenderer::MazeRenderer(Maze& maze) :
        visible{-1},
        maze(maze),
        compiled(new bool[maze.getChunksCount()] {false}),
        drawList(glGenLists(maze.getChunksCount())) {}

MazeRenderer::~MazeRenderer() {
    glDeleteLists(drawList, maze.getChunksCount());

    delete [] compiled;
}

void
MazeRenderer::update(float playerX, float playerY) {
    for (int i = 0; i < 16; i++)
        visible[i] = -1;

    for (int i = 0; i < maze.getChunksCount(); i++) {
        int chunkX = (i % maze.getChunksX());
        int chunkY = (i / maze.getChunksX());

        int pX = static_cast<int>(playerX) / Chunk::SIZE;
        int pY = static_cast<int>(playerY) / Chunk::SIZE;

        if     (((chunkX >= pX - 1 && chunkX <= pX + 1) && chunkY == pY) ||
                ((chunkY >= pY - 1 && chunkY <= pY + 1) && chunkX == pX))
            enableChunk(i);
    }
}

void
MazeRenderer::render() {
    for (int i = 0; i < 16; i++) {
        if (visible[i] != -1)
            renderChunk(visible[i]);
        else
            break;
    }
}

void
MazeRenderer::enableChunk(int num) {
    if (!compiled[num])
        compileChunk(num);

    for (int i = 0; i < 16; i++) {
        if (visible[i] == -1) {
            visible[i] = num;
            break;
        }
    }
}

void
MazeRenderer::renderChunk(int num) {
    glCallList(drawList + num);
}

}
