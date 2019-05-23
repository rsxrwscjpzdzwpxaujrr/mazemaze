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

namespace mazemaze {

MazeRenderer::MazeRenderer() = default;

MazeRenderer::~MazeRenderer() {
    glDeleteLists(drawList, 1);
}

void MazeRenderer::update(Maze* maze) {
    drawList = glGenLists(1);

    glNewList(drawList, GL_COMPILE);

    int mazeWidth = maze->getWidth();
    int mazeHeight = maze->getHeight();

    glBegin(GL_QUADS);

    glColor3f(1.0f, 0.0f, 1.0f);

    glVertex3i(0, 0, mazeHeight);
    glVertex3i(mazeWidth, 0, mazeHeight);
    glVertex3i(mazeWidth, 0, 0);
    glVertex3i(0, 0, 0);

    for (int i = 0; i < mazeHeight; i++)
        for (int j = 0; j < mazeWidth; j++)
            if (maze->getOpened(j, i)) {
                if (!maze->getOpened(j + 1, i)) {
                    glColor3f(1.0f, 0.0f, 0.0f);

                    glVertex3i(j + 1, 0, i + 1);
                    glVertex3i(j + 1, 1, i + 1);
                    glVertex3i(j + 1, 1, i);
                    glVertex3i(j + 1, 0, i);
                }

                if (!maze->getOpened(j - 1, i)) {
                    glColor3f(0.0f, 1.0f, 1.0f);

                    glVertex3i(j, 0, i);
                    glVertex3i(j, 1, i);
                    glVertex3i(j, 1, i + 1);
                    glVertex3i(j, 0, i + 1);
                }

                if (!maze->getOpened(j, i + 1)) {
                    glColor3f(0.0f, 0.0f, 1.0f);

                    glVertex3i(j, 0, i + 1);
                    glVertex3i(j, 1, i + 1);
                    glVertex3i(j + 1, 1, i + 1);
                    glVertex3i(j + 1, 0, i + 1);
                }

                if (!maze->getOpened(j, i - 1)) {
                    glColor3f(1.0f, 1.0f, 0.0f);

                    glVertex3i(j + 1, 0, i);
                    glVertex3i(j + 1, 1, i);
                    glVertex3i(j, 1, i);
                    glVertex3i(j, 0, i);
                }
            } else {
                glColor3f(0.0f, 1.0f, 0.0f);

                glVertex3i(j, 1, i);
                glVertex3i(j, 1, i + 1);
                glVertex3i(j + 1, 1, i + 1);
                glVertex3i(j + 1, 1, i);
            }

    glEnd();

    glEndList();
}

void
MazeRenderer::render() {
    glCallList(drawList);
}

}
