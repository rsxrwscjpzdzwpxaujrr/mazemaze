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

    glColor3f(0.5f, 0.5f, 0.5f);

    if (pos.x + Chunk::SIZE > maze.size().x)
        end.x = maze.size().x % Chunk::SIZE;

    if (pos.y + Chunk::SIZE > maze.size().y)
        end.y = maze.size().y % Chunk::SIZE;

    glNormal3f(0.0f, 1.0f, 0.0f);

    glVertex3i(0,     0, end.y);
    glVertex3i(end.x, 0, end.y);
    glVertex3i(end.x, 0, 0);
    glVertex3i(0,     0, 0);

    for (i.x = 0; i.x < end.x; i.x++)
        for (i.y = 0; i.y < end.y; i.y++)
            if (maze.get_opened(Point2i(i.x + pos.x, i.y + pos.y))) {
                if (!maze.get_opened(Point2i(i.x + 1 + pos.x, i.y + pos.y))) {
                    glNormal3f(-1.0f, 0.0f, 0.0f);

                    glVertex3i(i.x + 1, 0, i.y + 1);
                    glVertex3i(i.x + 1, 1, i.y + 1);
                    glVertex3i(i.x + 1, 1, i.y);
                    glVertex3i(i.x + 1, 0, i.y);
                }

                if (!maze.get_opened(Point2i(i.x - 1 + pos.x, i.y + pos.y))) {
                    glNormal3f(1.0f, 0.0f, 0.0f);

                    glVertex3i(i.x, 0, i.y);
                    glVertex3i(i.x, 1, i.y);
                    glVertex3i(i.x, 1, i.y + 1);
                    glVertex3i(i.x, 0, i.y + 1);
                }

                if (!maze.get_opened(Point2i(i.x + pos.x, i.y + 1 + pos.y))) {
                    glNormal3f(0.0f, 0.0f, -1.0f);

                    glVertex3i(i.x, 0, i.y + 1);
                    glVertex3i(i.x, 1, i.y + 1);
                    glVertex3i(i.x + 1, 1, i.y + 1);
                    glVertex3i(i.x + 1, 0, i.y + 1);
                }

                if (!maze.get_opened(Point2i(i.x + pos.x, i.y - 1 + pos.y))) {
                    glNormal3f(0.0f, 0.0f, 1.0f);

                    glVertex3i(i.x + 1, 0, i.y);
                    glVertex3i(i.x + 1, 1, i.y);
                    glVertex3i(i.x, 1, i.y);
                    glVertex3i(i.x, 0, i.y);
                }
            } else {
                glNormal3f(0.0f, 1.0f, 0.0f);

                glVertex3i(i.x, 1, i.y);
                glVertex3i(i.x, 1, i.y + 1);
                glVertex3i(i.x + 1, 1, i.y + 1);
                glVertex3i(i.x + 1, 1, i.y);
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
    float light0_diffuse[] = {1.0f, 0.9f, 0.8f};
    float light0_ambient[] = {0.5f, 0.55f, 0.75f};
    float light0_position[] = {0.5f, 0.75f, 0.25f, 0.0f};

    glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
    glLightfv(GL_LIGHT0, GL_POSITION, light0_position);

    glEnable(GL_LIGHTING);
    MazeRenderer::render_chunks(chunks);
    glDisable(GL_LIGHTING);
}

void
Gray::render_sky() {
    skybox.render();
}

}
}
