/*
 * Copyright (c) 2019-2021, Мира Странная <rsxrwscjpzdzwpxaujrr@yahoo.com>
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

#include "../Logger.hpp"
#include "../utils.hpp"
#include "../Chunk.hpp"
#include "../Game.hpp"
#include "../Camera.hpp"

namespace mazemaze {
namespace renderers {

Gray::Gray(Game& game) :
        MazeRenderer(game),
        game(game),
        skybox(50, 0.67f, 0.85f, 1.0f) {}

Gray::~Gray() = default;

void
Gray::set_states() {
    glEnable(GL_LIGHT0);
}

void
Gray::on_disable() {
    glDisable(GL_LIGHT0);
}

void
Gray::compile_chunk(int num) {
    int x = (num % maze.get_chunks_x()) * Chunk::SIZE;
    int y = (num / maze.get_chunks_x()) * Chunk::SIZE;

    Logger::inst().log_debug(fmt("Compiling chunk %d at %d %d.", num, x, y));

    glNewList(draw_list + num, GL_COMPILE);

    glPushMatrix();

    glTranslatef(x, 0.0, y);

    glBegin(GL_QUADS);

    glColor3f(0.5f, 0.5f, 0.5f);

    int endX = Chunk::SIZE;
    int endY = Chunk::SIZE;

    if (x + Chunk::SIZE > maze.get_width())
        endX = maze.get_width() % Chunk::SIZE;

    if (y + Chunk::SIZE > maze.get_height())
        endY = maze.get_height() % Chunk::SIZE;

    glNormal3f(0.0f, 1.0f, 0.0f);

    glVertex3i(0,    0, endY);
    glVertex3i(endX, 0, endY);
    glVertex3i(endX, 0, 0);
    glVertex3i(0,    0, 0);

    for (int j = 0; j < endX; j++)
        for (int k = 0; k < endY; k++)
            if (maze.get_opened(j + x, k + y)) {
                if (!maze.get_opened(j + 1 + x, k + y)) {
                    glNormal3f(-1.0f, 0.0f, 0.0f);

                    glVertex3i(j + 1, 0, k + 1);
                    glVertex3i(j + 1, 1, k + 1);
                    glVertex3i(j + 1, 1, k);
                    glVertex3i(j + 1, 0, k);
                }

                if (!maze.get_opened(j - 1 + x, k + y)) {
                    glNormal3f(1.0f, 0.0f, 0.0f);

                    glVertex3i(j, 0, k);
                    glVertex3i(j, 1, k);
                    glVertex3i(j, 1, k + 1);
                    glVertex3i(j, 0, k + 1);
                }

                if (!maze.get_opened(j + x, k + 1 + y)) {
                    glNormal3f(0.0f, 0.0f, -1.0f);

                    glVertex3i(j, 0, k + 1);
                    glVertex3i(j, 1, k + 1);
                    glVertex3i(j + 1, 1, k + 1);
                    glVertex3i(j + 1, 0, k + 1);
                }

                if (!maze.get_opened(j + x, k - 1 + y)) {
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
Gray::on_tick(float) {
}

void
Gray::render_chunks(int chunks[]) {
    Camera& camera = game.get_player().get_camera();

    float light0_diffuse[] = {1.0f, 0.9f, 0.8f};
    float light0_ambient[] = {0.5f, 0.55f, 0.75f};
    float light0_position[] = {0.5f, 0.75f, 0.25f, 0.0f};

    glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
    glLightfv(GL_LIGHT0, GL_POSITION, light0_position);

    glEnable(GL_LIGHTING);

    MazeRenderer::render_chunks(chunks);

    glDisable(GL_LIGHTING);

    glTranslatef(camera.get_x(), camera.get_y(), camera.get_z());
    skybox.render();
    glTranslatef(-camera.get_x(), -camera.get_y(), -camera.get_z());
}

}
}
