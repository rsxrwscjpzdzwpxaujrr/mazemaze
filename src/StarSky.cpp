/*
 * Copyright (c) 2018-2019, Мира Странная <miraityan2004@gmail.com>
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

#include <random>
#include <cmath>

#include <SFML/OpenGL.hpp>

#include "StarSky.hpp"

namespace mazemaze {

StarSky::StarSky(int starCount, float timeSpeed, float pitch, float yaw) {
    StarSky::timeSpeed = timeSpeed;

    StarSky::pitch = pitch;
    StarSky::yaw = yaw;

    if (!firstGeneration) {
        delete starsX;
        delete starsY;
        delete starsZ;
        delete starSize;
    }

    starsX = new float[static_cast<uint>(starCount)];
    starsY = new float[static_cast<uint>(starCount)];
    starsZ = new float[static_cast<uint>(starCount)];
    starSize = new int[static_cast<uint>(starCount)];

    firstGeneration = false;

    StarSky::starCount = starCount;

    float randX;
    float randY;
    float randZ;
    float distance;
    int sizeRand;

    drawList = glGenLists(1);
    glNewList(drawList, GL_COMPILE);

    std::mt19937 randGen(0);
    std::uniform_real_distribution<> coordInterval(-1.0, 1.0);
    std::uniform_int_distribution<> sizeInterval(0, 11);

    for (int i = 0; i < starCount; i++) {
        randX = coordInterval(randGen);
        randY = coordInterval(randGen);
        randZ = coordInterval(randGen);
        sizeRand = sizeInterval(randGen);

        distance = std::sqrt(randX * randX + randY * randY + randZ * randZ);

        if (distance > 1.0f) {
            i--;
            continue;
        }

        starsX[i] = randX / distance;
        starsY[i] = randY / distance;
        starsZ[i] = randZ / distance;

        if (sizeRand <= 7) {
            starSize[i] = 1;
            glColor4f(0.33f, 0.33f, 0.33f, 1.0f);
        } else if (sizeRand <= 10) {
            starSize[i] = 2;
            glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
        } else {
            starSize[i] = 3;
            glColor4f(0.75f, 0.75f, 0.75f, 1.0f);
        }

        glPointSize(starSize[i]);
        glBegin(GL_POINTS);
        glVertex3f(starsX[i], starsY[i], starsZ[i]);
        glEnd();
    }

    glEndList();
}

StarSky::~StarSky() {
    if (!firstGeneration) {
        delete starsX;
        delete starsY;
        delete starsZ;
        delete starSize;
    }

    glDeleteLists(drawList, 1);
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

void StarSky::tick(float deltatime) {
    time += deltatime * timeSpeed;
}

void StarSky::setTime(float time) {
    StarSky::time = time;
}

}
