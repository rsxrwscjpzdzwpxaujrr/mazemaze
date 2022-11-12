/*
 * Copyright (c) 2018-2022, Мира Странная <rsxrwscjpzdzwpxaujrr@yahoo.com>
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

#include "utils.hpp"

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
        Pointf rand(
            coord_interval(rand_gen),
            coord_interval(rand_gen),
            coord_interval(rand_gen)
        );

        int size_rand = size_interval(rand_gen);

        float distance = powfi(rand.x, 2) + powfi(rand.y, 2) + powfi(rand.z, 2);

        if (distance <= 1.0f) {
            distance = std::sqrt(distance);

            star->x = rand.x / distance;
            star->y = rand.y / distance;
            star->z = rand.z / distance;

            if      (size_rand <= 7)
                star->size = 1;

            else if (size_rand <= 10)
                star->size = 2;

            else
                star->size = 3;

            star++;
        }
    }

    std::normal_distribution<float> temp_interval(7300.0f, 1500.0f);

    for (auto& star : stars) {
        float temp = temp_interval(rand_gen);

        auto color = temp_to_color(temp);

        star.r = color.x;
        star.g = color.y;
        star.b = color.z;
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
            glColor3f(star->r * 0.33f, star->g * 0.33f, star->b * 0.33f);

        else if (star->size == 2)
            glColor3f(star->r * 0.5f,  star->g * 0.5f,  star->b * 0.5f);

        else if (star->size == 3)
            glColor3f(star->r * 0.75f, star->g * 0.75f, star->b * 0.75f);

        glPointSize(star->size);
        glBegin(GL_POINTS);
        glVertex3f(star->x, star->y, star->z);
        glEnd();
    }

    glEndList();
}

Pointf
StarSky::temp_to_color(float temp) const {
    // https://tannerhelland.com/2012/09/18/convert-temperature-rgb-algorithm-code.html

    if (temp < 1000)
        temp = 1000;

    if (temp > 40000)
        temp = 40000;

    float r = 0.0f;
    float g = 0.0f;
    float b = 0.0f;

    temp /= 100.0f;

    if (temp <= 66.0f) {
        r = 1.0f;
    } else {
        r = 1.287885654f * std::pow(temp - 60.0f, -0.1332047592f);

        if (r > 1.0f)
            r = 1.0f;

        if (r < 0.0f)
            r = 0.0f;
    }

    if (temp <= 66.0f) {
        g = 0.388557823f * std::log(temp) - 0.629373313f;
    } else {
        g = 1.125477225f * std::pow(temp - 60.0f, -0.0755148492f);
    }

    if (g > 1.0f)
        g = 1.0f;

    if (g < 0.0f)
        g = 0.0f;

    if (temp >= 66.0f) {
        b = 1.0f;
    } else if (temp <= 19.0f) {
        b = 0.0f;
    } else {
        b = 0.541084888f * std::log(temp - 10.0f) - 1.191581222f;

        if (b > 1.0f)
            b = 1.0f;

        if (b < 0.0f)
            b = 0.0f;
    }

    return Pointf(r, g, b);
}

}
