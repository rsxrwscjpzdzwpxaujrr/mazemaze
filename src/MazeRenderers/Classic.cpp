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
    Point2i end(Chunk::SIZE, Chunk::SIZE);
    Point2i i;
    Point2i pos(
        (num % maze.chunks_count().x) * Chunk::SIZE,
        (num / maze.chunks_count().x) * Chunk::SIZE
    );

    Logger::inst().log_debug(fmt("Compiling chunk %d at %d %d.", num, pos.x, pos.y));

    glNewList(draw_list + num, GL_COMPILE);

    glPushMatrix();

    glTranslatef(pos.x, 0.0, pos.y);

    glBegin(GL_QUADS);

    glColor3f(1.0f, 0.0f, 1.0f);

    if (pos.x + Chunk::SIZE > maze.size().x)
        end.x = maze.size().x % Chunk::SIZE;

    if (pos.y + Chunk::SIZE > maze.size().y)
        end.y = maze.size().y % Chunk::SIZE;

    glVertex3i(0,     0, end.y);
    glVertex3i(end.x, 0, end.y);
    glVertex3i(end.x, 0, 0);
    glVertex3i(0,     0, 0);

    for (i.x = 0; i.x < end.x; i.x++)
        for (i.y = 0; i.y < end.y; i.y++)
            if (maze.get_opened(Point2i(i.x + pos.x, i.y + pos.y))) {
                if (!maze.get_opened(Point2i(i.x + 1 + pos.x, i.y + pos.y))) {
                    glColor3f(1.0f, 0.0f, 0.0f);

                    glVertex3i(i.x + 1, 0, i.y + 1);
                    glVertex3i(i.x + 1, 1, i.y + 1);
                    glVertex3i(i.x + 1, 1, i.y);
                    glVertex3i(i.x + 1, 0, i.y);
                }

                if (!maze.get_opened(Point2i(i.x - 1 + pos.x, i.y + pos.y))) {
                    glColor3f(0.0f, 1.0f, 1.0f);

                    glVertex3i(i.x, 0, i.y);
                    glVertex3i(i.x, 1, i.y);
                    glVertex3i(i.x, 1, i.y + 1);
                    glVertex3i(i.x, 0, i.y + 1);
                }

                if (!maze.get_opened(Point2i(i.x + pos.x, i.y + 1 + pos.y))) {
                    glColor3f(0.0f, 0.0f, 1.0f);

                    glVertex3i(i.x,     0, i.y + 1);
                    glVertex3i(i.x,     1, i.y + 1);
                    glVertex3i(i.x + 1, 1, i.y + 1);
                    glVertex3i(i.x + 1, 0, i.y + 1);
                }

                if (!maze.get_opened(Point2i(i.x + pos.x, i.y - 1 + pos.y))) {
                    glColor3f(1.0f, 1.0f, 0.0f);

                    glVertex3i(i.x + 1, 0, i.y);
                    glVertex3i(i.x + 1, 1, i.y);
                    glVertex3i(i.x,     1, i.y);
                    glVertex3i(i.x,     0, i.y);
                }
            } else {
                glColor3f(0.0f, 1.0f, 0.0f);

                glVertex3i(i.x,     1, i.y);
                glVertex3i(i.x,     1, i.y + 1);
                glVertex3i(i.x + 1, 1, i.y + 1);
                glVertex3i(i.x + 1, 1, i.y);
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
