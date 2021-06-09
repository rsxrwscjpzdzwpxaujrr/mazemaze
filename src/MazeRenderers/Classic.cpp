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
        star_sky(1024, 0.0f, Rotation(1.5f, 0.7f, 0.0f)),
        game(game) {}

Classic::~Classic() = default;

void
Classic::compile_chunk(int num) {
    int x = (num % maze.get_chunks_x()) * Chunk::SIZE;
    int y = (num / maze.get_chunks_x()) * Chunk::SIZE;

    Logger::inst().log_debug(fmt("Compiling chunk %d at %d %d.", num, x, y));

    glNewList(draw_list + num, GL_COMPILE);

    glPushMatrix();

    glTranslatef(x, 0.0, y);

    glBegin(GL_QUADS);

    glColor3f(1.0f, 0.0f, 1.0f);

    int end_x = Chunk::SIZE;
    int end_y = Chunk::SIZE;

    if (x + Chunk::SIZE > maze.get_width())
        end_x = maze.get_width() % Chunk::SIZE;

    if (y + Chunk::SIZE > maze.get_height())
        end_y = maze.get_height() % Chunk::SIZE;

    glVertex3i(0,     0, end_y);
    glVertex3i(end_x, 0, end_y);
    glVertex3i(end_x, 0, 0);
    glVertex3i(0,     0, 0);

    for (int j = 0; j < end_x; j++)
        for (int k = 0; k < end_y; k++)
            if (maze.get_opened(j + x, k + y)) {
                if (!maze.get_opened(j + 1 + x, k + y)) {
                    glColor3f(1.0f, 0.0f, 0.0f);

                    glVertex3i(j + 1, 0, k + 1);
                    glVertex3i(j + 1, 1, k + 1);
                    glVertex3i(j + 1, 1, k);
                    glVertex3i(j + 1, 0, k);
                }

                if (!maze.get_opened(j - 1 + x, k + y)) {
                    glColor3f(0.0f, 1.0f, 1.0f);

                    glVertex3i(j, 0, k);
                    glVertex3i(j, 1, k);
                    glVertex3i(j, 1, k + 1);
                    glVertex3i(j, 0, k + 1);
                }

                if (!maze.get_opened(j + x, k + 1 + y)) {
                    glColor3f(0.0f, 0.0f, 1.0f);

                    glVertex3i(j, 0, k + 1);
                    glVertex3i(j, 1, k + 1);
                    glVertex3i(j + 1, 1, k + 1);
                    glVertex3i(j + 1, 0, k + 1);
                }

                if (!maze.get_opened(j + x, k - 1 + y)) {
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
Classic::on_tick(float) {
    star_sky.set_time(game.get_time());
}

void
Classic::render_sky() {
    star_sky.render();
}

}
}
