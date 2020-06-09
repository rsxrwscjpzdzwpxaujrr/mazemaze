/*
 * Copyright (c) 2018-2019, Мира Странная <rsxrwscjpzdzwpxaujrr@yahoo.com>
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

StarSky::StarSky(int starCount, float timeSpeed, float pitch, float yaw) :
        starCount(starCount),
        distance(90.0f),
        pitch(pitch),
        yaw(yaw),
        time(0.0f),
        timeSpeed(timeSpeed),
        stars(new Star[starCount]) {
    std::mt19937 randGen(0);
    std::uniform_real_distribution<float> coordInterval(-1.0, 1.0);
    std::uniform_int_distribution<> sizeInterval(0, 11);

    for (int i = 0; i < starCount;) {
        float randX = coordInterval(randGen);
        float randY = coordInterval(randGen);
        float randZ = coordInterval(randGen);
        int sizeRand = sizeInterval(randGen);

        float distance = std::sqrt(randX * randX + randY * randY + randZ * randZ);

        if (distance <= 1.0f) {
            Star& star = stars[i];

            star.x = randX / distance;
            star.y = randY / distance;
            star.z = randZ / distance;

            if      (sizeRand <= 7)
                star.size = 1;

            else if (sizeRand <= 10)
                star.size = 2;

            else
                star.size = 3;

            i++;
        }
    }

    compile();
}

StarSky::~StarSky() {
    glDeleteLists(drawList, 1);

    delete [] stars;
}

void
StarSky::render() {
    glPushMatrix();

    roll = time * static_cast<float>((2.0 * M_PI) / 24.0 / 60.0 / 60.0);

    glRotatef(pitch * static_cast<float>(180.0 / M_PI), 1.0f, 0.0f, 0.0f);
    glRotatef(yaw * static_cast<float>(180.0 / M_PI), 0.0f, 1.0f, 0.0f);
    glRotatef(roll * static_cast<float>(180.0 / M_PI), 0.0f, 0.0f, 1.0f);

    glScalef(distance, distance, distance);

    glCallList(drawList);

    glPopMatrix();
}

void
StarSky::tick(float deltatime) {
    time += deltatime * timeSpeed;
}

void
StarSky::setTime(float time) {
    StarSky::time = time;
}

void
StarSky::compile() {
    drawList = glGenLists(1);
    glNewList(drawList, GL_COMPILE);

    for (int i = 0; i < starCount; i++) {
        Star& star = stars[i];

        if      (star.size == 1)
            glColor3f(0.33f, 0.33f, 0.33f);

        else if (star.size == 2)
            glColor3f(0.5f,  0.5f,  0.5f);

        else if (star.size == 3)
            glColor3f(0.75f, 0.75f, 0.75f);

        glPointSize(star.size);
        glBegin(GL_POINTS);
        glVertex3f(star.x, star.y, star.z);
        glEnd();
    }

    glEndList();
}

}
