/*
 * Copyright (c) 2019, Мира Странная <miraityan2004@gmail.com>
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

#include "Gray.hpp"

#include <SFML/OpenGL.hpp>

#include "../Chunk.hpp"
#include "../Game.hpp"
#include "../Camera.hpp"

namespace mazemaze {
namespace renderers {

Gray::Gray(Game& game) :
        MazeRenderer(game),
        game(game),
        skybox(50, 0.67f, 0.85f, 1.0f) {
    glEnable(GL_LIGHT0);
}

Gray::~Gray() {
    glDisable(GL_LIGHT0);
}

void
Gray::compileChunk(int num) {
    glNewList(drawList + num, GL_COMPILE);

    glPushMatrix();

    int x = (num % maze.getChunksX()) * Chunk::SIZE;
    int y = (num / maze.getChunksX()) * Chunk::SIZE;

    glTranslatef(x, 0.0, y);

    glBegin(GL_QUADS);

    glColor3f(0.5f, 0.5f, 0.5f);

    int endX = Chunk::SIZE;
    int endY = Chunk::SIZE;

    if (x + Chunk::SIZE > maze.getWidth())
        endX = maze.getWidth() % Chunk::SIZE;

    if (y + Chunk::SIZE > maze.getHeight())
        endY = maze.getHeight() % Chunk::SIZE;

    glNormal3f(0.0f, 1.0f, 0.0f);

    glVertex3i(0,    0, endY);
    glVertex3i(endX, 0, endY);
    glVertex3i(endX, 0, 0);
    glVertex3i(0,    0, 0);

    for (int j = 0; j < endX; j++)
        for (int k = 0; k < endY; k++)
            if (maze.getOpened(j + x, k + y)) {
                if (!maze.getOpened(j + 1 + x, k + y)) {
                    glNormal3f(-1.0f, 0.0f, 0.0f);

                    glVertex3i(j + 1, 0, k + 1);
                    glVertex3i(j + 1, 1, k + 1);
                    glVertex3i(j + 1, 1, k);
                    glVertex3i(j + 1, 0, k);
                }

                if (!maze.getOpened(j - 1 + x, k + y)) {
                    glNormal3f(1.0f, 0.0f, 0.0f);

                    glVertex3i(j, 0, k);
                    glVertex3i(j, 1, k);
                    glVertex3i(j, 1, k + 1);
                    glVertex3i(j, 0, k + 1);
                }

                if (!maze.getOpened(j + x, k + 1 + y)) {
                    glNormal3f(0.0f, 0.0f, -1.0f);

                    glVertex3i(j, 0, k + 1);
                    glVertex3i(j, 1, k + 1);
                    glVertex3i(j + 1, 1, k + 1);
                    glVertex3i(j + 1, 0, k + 1);
                }

                if (!maze.getOpened(j + x, k - 1 + y)) {
                    glNormal3f(0.0f, 0.0f, 1.0f);

                    glVertex3i(j + 1, 0, k);
                    glVertex3i(j + 1, 1, k);
                    glVertex3i(j, 1, k);
                    glVertex3i(j, 0, k);
                }
            } else {
                glNormal3f(0.0f, 1.0f, 0.0f);

                glVertex3i(j, 1, k);
                glVertex3i(j, 1, k + 1);
                glVertex3i(j + 1, 1, k + 1);
                glVertex3i(j + 1, 1, k);
            }

    glEnd();

    glPopMatrix();

    glEndList();

    compiled[num] = true;
}

void
Gray::onTick(float deltaTime) {
}

void
Gray::renderChunk(int num) {
    Camera& camera = game.getPlayer().getCamera();

    float light0_diffuse[] = {1.0f, 0.9f, 0.8f};
    float light0_ambient[] = {0.5f, 0.55f, 0.75f};
    float light0_position[] = {0.5f, 0.75f, 0.25f, 0.0f};

    glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
    glLightfv(GL_LIGHT0, GL_POSITION, light0_position);

    glEnable(GL_LIGHTING);

    MazeRenderer::renderChunk(num);

    glDisable(GL_LIGHTING);

    glTranslatef(camera.getX(), camera.getY(), camera.getZ());
    skybox.render();
    glTranslatef(-camera.getX(), -camera.getY(), -camera.getZ());
}

}
}
