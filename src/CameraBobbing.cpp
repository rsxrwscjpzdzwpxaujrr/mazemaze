/*
 * Copyright (c) 2021, Мира Странная <rsxrwscjpzdzwpxaujrr@yahoo.com>
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

#include "CameraBobbing.hpp"

#include "Camera.hpp"
#include "Player.hpp"

#include <cmath>
#include <algorithm>

namespace mazemaze {

CameraBobbing::CameraBobbing(Camera& camera, Player& player) :
    camera(camera),
    player(player),
    pitchCoeff(0.0f),
    posCoeff(0.0f),
    timeCoeff(0.0f),
    lastPitch(0.0f),
    easingType(false),
    time(0.0f) {
}

CameraBobbing::~CameraBobbing() = default;

void
CameraBobbing::tick(float deltaTime) {
    float pitch = camera.getPitch();
    float yaw   = camera.getYaw();

    float x = camera.getX();
    float y = camera.getY();
    float z = camera.getZ();

    float currentPitch;
    float currentX;
    float currentY;

    float pitchCoeffWithEasing;
    float posCoeffWithEasing;
    float timeCoeffWithEasing;

    float (*easingFunc)(float);

    if (player.isMoving()) {
        if (pitchCoeff < 1.0f)
            pitchCoeff += std::min(deltaTime * 5.0f, 1.0f - pitchCoeff);

        if (  posCoeff < 1.0f)
            posCoeff   += std::min(deltaTime * 5.0f, 1.0f -   posCoeff);

        if ( timeCoeff < 1.0f)
            timeCoeff  += std::min(deltaTime * 5.0f, 1.0f -  timeCoeff);

        easingType = true;
    }

    if (!player.isMoving()) {
        if (pitchCoeff > 0.0f)
            pitchCoeff -= std::min(deltaTime * 2.0f, pitchCoeff);

        if (  posCoeff > 0.0f)
            posCoeff   -= std::min(deltaTime * 3.0f,   posCoeff);

        if ( timeCoeff > 0.0f)
            timeCoeff  -= std::min(deltaTime * 3.0f,  timeCoeff);

        easingType = false;
    }

    if (easingType) {
        easingFunc = &easeOutCubic;
    } else {
        easingFunc = &easeInCubic;
    }

    pitchCoeffWithEasing = easingFunc(pitchCoeff);
      posCoeffWithEasing = easingFunc(  posCoeff);
     timeCoeffWithEasing = easingFunc( timeCoeff);

    currentPitch = (std::cos(time * 22.0f) + 1.0f) * 0.002f * pitchCoeffWithEasing;

    currentX =           std::cos(time * 11.0f)  * 0.02f * posCoeffWithEasing;
    currentY = -std::abs(std::sin(time * 11.0f)) * 0.02f * posCoeffWithEasing;

    camera.setPitch(pitch - lastPitch + currentPitch);

    camera.setX(x + currentX * std::cos(yaw));
    camera.setY(y + currentY);
    camera.setZ(z + currentX * std::sin(yaw));

    lastPitch = currentPitch;

    time += deltaTime * timeCoeffWithEasing;
}

float
CameraBobbing::easeOutCubic(float x) {
    return 1.0f - easeInCubic(1.0f - x);
}

float
CameraBobbing::easeInCubic(float x) {
    return x * x * x;
}

}


