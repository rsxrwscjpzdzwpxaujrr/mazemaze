/*
 * Copyright (c) 2019-2020, Мира Странная <rsxrwscjpzdzwpxaujrr@yahoo.com>
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

#include "Classic.hpp"

#include <SFML/OpenGL.hpp>

#include "../Logger.hpp"
#include "../utils.hpp"
#include "../Chunk.hpp"
#include "../Game.hpp"
#include "../Camera.hpp"

namespace mazemaze {
namespace renderers {

Classic::Classic(Game& game) :
        MazeRenderer(game),
        starSky(1024, 0.0f, 1.5f, 0.7f),
        game(game) {}

Classic::~Classic() = default;

void
Classic::compileChunk(int num) {
    int x = (num % maze.getChunksX()) * Chunk::SIZE;
    int y = (num / maze.getChunksX()) * Chunk::SIZE;

    Logger::inst().log_debug(fmt("Compiling chunk %d at %d %d.", num, x, y));

    glNewList(drawList + num, GL_COMPILE);

    glPushMatrix();

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

    compiled[num] = true;
}

void
Classic::onTick(float) {
    starSky.setTime(game.getTime());
}

void
Classic::renderChunks(int chunks[]) {
    Camera& camera = game.getPlayer().getCamera();

    MazeRenderer::renderChunks(chunks);

    glTranslatef(camera.getX(), camera.getY(), camera.getZ());
    starSky.render();
    glTranslatef(-camera.getX(), -camera.getY(), -camera.getZ());
}

}
}
