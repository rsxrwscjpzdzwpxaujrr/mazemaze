/*
 * Copyright (c) 2019, Мира Странная <rsxrwscjpzdzwpxaujrr@yahoo.com>
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

#include "Skybox.hpp"

#include <SFML/OpenGL.hpp>

namespace mazemaze {

Skybox::Skybox(int size, float red, float green, float blue) :
        size(size),
        red(red),
        green(green),
        blue(blue),
        list(0) {}

Skybox::~Skybox() = default;

void
Skybox::render() {
    if (list == 0)
        compile();

    glCallList(list);
}

void
Skybox::compile() {
    list = glGenLists(1);

    glNewList(list, GL_COMPILE);

    glColor3f(red, green, blue);

    glBegin(GL_QUADS);
    glVertex3i( size, -size, -size);
    glVertex3i( size, -size,  size);
    glVertex3i( size,  size,  size);
    glVertex3i( size,  size, -size);

    glVertex3i(-size,  size, -size);
    glVertex3i(-size,  size,  size);
    glVertex3i(-size, -size,  size);
    glVertex3i(-size, -size, -size);

    glVertex3i( size,  size, -size);
    glVertex3i( size,  size,  size);
    glVertex3i(-size,  size,  size);
    glVertex3i(-size,  size, -size);

    glVertex3i(-size, -size, -size);
    glVertex3i(-size, -size,  size);
    glVertex3i( size, -size,  size);
    glVertex3i( size, -size, -size);

    glVertex3i(-size, -size,  size);
    glVertex3i(-size,  size,  size);
    glVertex3i( size,  size,  size);
    glVertex3i( size, -size,  size);

    glVertex3i( size, -size, -size);
    glVertex3i( size,  size, -size);
    glVertex3i(-size,  size, -size);
    glVertex3i(-size, -size, -size);
    glEnd();

    glEndList();
}

}
