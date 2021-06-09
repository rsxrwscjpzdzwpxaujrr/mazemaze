/*
 * Copyright (c) 2018-2021, Мира Странная <rsxrwscjpzdzwpxaujrr@yahoo.com>
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

#include "StarSky.hpp"

#include <random>
#include <cmath>

#include <SFML/OpenGL.hpp>

namespace mazemaze {

StarSky::StarSky(int star_count, float time_speed, Rotation rotation) :
        star_count(star_count),
        distance(90.0f),
        m_rotation(rotation),
        time(0.0f),
        time_speed(time_speed),
        stars(star_count) {
    std::mt19937 rand_gen(0);
    std::uniform_real_distribution<float> coord_interval(-1.0, 1.0);
    std::uniform_int_distribution<> size_interval(0, 11);

    for (auto star = stars.begin(); star < stars.end();) {
        float rand_x = coord_interval(rand_gen);
        float rand_y = coord_interval(rand_gen);
        float rand_z = coord_interval(rand_gen);
        int size_rand = size_interval(rand_gen);

        float distance = rand_x * rand_x + rand_y * rand_y + rand_z * rand_z;

        if (distance <= 1.0f) {
            distance = std::sqrt(distance);

            star->x = rand_x / distance;
            star->y = rand_y / distance;
            star->z = rand_z / distance;

            if      (size_rand <= 7)
                star->size = 1;

            else if (size_rand <= 10)
                star->size = 2;

            else
                star->size = 3;

            star++;
        }
    }

    compile();
}

StarSky::~StarSky() {
    glDeleteLists(draw_list, 1);
}

void
StarSky::render() {
    glPushMatrix();

    float todeg = static_cast<float>(180.0 / M_PI);

    m_rotation.set_roll(time * static_cast<float>((2.0 * M_PI) / 24.0 / 60.0 / 60.0));

    glRotatef(m_rotation.pitch() * todeg, 1.0f, 0.0f, 0.0f);
    glRotatef(m_rotation.yaw()   * todeg, 0.0f, 1.0f, 0.0f);
    glRotatef(m_rotation.roll()  * todeg, 0.0f, 0.0f, 1.0f);

    glScalef(distance, distance, distance);

    glCallList(draw_list);

    glPopMatrix();
}

void
StarSky::tick(void*, float delta_time) {
    time += delta_time * time_speed;
}

void
StarSky::set_time(float time) {
    StarSky::time = time;
}

void
StarSky::compile() {
    draw_list = glGenLists(1);
    glNewList(draw_list, GL_COMPILE);

    for (auto star = stars.begin(); star < stars.end(); star++) {
        if      (star->size == 1)
            glColor3f(0.33f, 0.33f, 0.33f);

        else if (star->size == 2)
            glColor3f(0.5f,  0.5f,  0.5f);

        else if (star->size == 3)
            glColor3f(0.75f, 0.75f, 0.75f);

        glPointSize(star->size);
        glBegin(GL_POINTS);
        glVertex3f(star->x, star->y, star->z);
        glEnd();
    }

    glEndList();
}

}
