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

#include <algorithm>

#include <SFML/OpenGL.hpp>

#include "Chunk.hpp"

namespace mazemaze {

MazeRenderer::MazeRenderer(Maze& maze) : maze(maze) {}

MazeRenderer::~MazeRenderer() {
    glDeleteLists(drawList, maze.getChunksCount());
}

void MazeRenderer::update() {
    drawList = glGenLists(maze.getChunksCount());

    for (int i = 0; i < maze.getChunksCount(); i++)
        compileChunk(i);
}

void
MazeRenderer::render(float playerX, float playerY) {
    for (int i = 0; i < maze.getChunksCount(); i++) {
        int chunkX = (i % maze.getChunksX());
        int chunkY = (i / maze.getChunksX());

        int pX = static_cast<int>(playerX) / Chunk::SIZE;
        int pY = static_cast<int>(playerY) / Chunk::SIZE;

        if     (((chunkX >= pX - 1 && chunkX <= pX + 1) && chunkY == pY) ||
                ((chunkY >= pY - 1 && chunkY <= pY + 1) && chunkX == pX))
            renderChunk(i);
    }
}

void
MazeRenderer::compileChunk(int num) {
    glNewList(drawList + num, GL_COMPILE);

    glPushMatrix();

    int x = (num % maze.getChunksX()) * Chunk::SIZE;
    int y = (num / maze.getChunksX()) * Chunk::SIZE;

    glTranslatef(x, 0.0, y);

    glBegin(GL_QUADS);

    glColor3f(1.0f, 0.0f, 1.0f);

    int endX = Chunk::SIZE;
    int endY = Chunk::SIZE;

    if (x + Chunk::SIZE > maze.getWidth())
        endX = maze.getWidth() % Chunk::SIZE;

    if (y + Chunk::SIZE > maze.getHeight())
        endY = maze.getHeight() % Chunk::SIZE;

    glVertex3i(0,    0, endY);
    glVertex3i(endX, 0, endY);
    glVertex3i(endX, 0, 0);
    glVertex3i(0,    0, 0);

    for (int j = 0; j < endX; j++)
        for (int k = 0; k < endY; k++)
            if (maze.getOpened(j + x, k + y)) {
                if (!maze.getOpened(j + 1 + x, k + y)) {
                    glColor3f(1.0f, 0.0f, 0.0f);

                    glVertex3i(j + 1, 0, k + 1);
                    glVertex3i(j + 1, 1, k + 1);
                    glVertex3i(j + 1, 1, k);
                    glVertex3i(j + 1, 0, k);
                }

                if (!maze.getOpened(j - 1 + x, k + y)) {
                    glColor3f(0.0f, 1.0f, 1.0f);

                    glVertex3i(j, 0, k);
                    glVertex3i(j, 1, k);
                    glVertex3i(j, 1, k + 1);
                    glVertex3i(j, 0, k + 1);
                }

                if (!maze.getOpened(j + x, k + 1 + y)) {
                    glColor3f(0.0f, 0.0f, 1.0f);

                    glVertex3i(j, 0, k + 1);
                    glVertex3i(j, 1, k + 1);
                    glVertex3i(j + 1, 1, k + 1);
                    glVertex3i(j + 1, 0, k + 1);
                }

                if (!maze.getOpened(j + x, k - 1 + y)) {
                    glColor3f(1.0f, 1.0f, 0.0f);

                    glVertex3i(j + 1, 0, k);
                    glVertex3i(j + 1, 1, k);
                    glVertex3i(j, 1, k);
                    glVertex3i(j, 0, k);
                }
            } else {
                glColor3f(0.0f, 1.0f, 0.0f);

                glVertex3i(j, 1, k);
                glVertex3i(j, 1, k + 1);
                glVertex3i(j + 1, 1, k + 1);
                glVertex3i(j + 1, 1, k);
            }

    glEnd();

    glPopMatrix();

    glEndList();
}

void
MazeRenderer::renderChunk(int num) {
    glCallList(drawList + num);
}

}
