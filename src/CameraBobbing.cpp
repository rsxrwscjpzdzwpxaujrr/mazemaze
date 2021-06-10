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

const float  rot_out_speed = 2.0f;
const float  pos_out_speed = 3.0f;
const float time_out_speed = 3.0f;

const float  rot_in_speed = 5.0f;
const float  pos_in_speed = 5.0f;
const float time_in_speed = 5.0f;

CameraBobbing::CameraBobbing() :
    rot_coeff(0.0f),
    pos_coeff(0.0f),
    time_coeff(0.0f),
    last_pitch(0.0f),
    easing_type(false),
    time(0.0f) {
}

CameraBobbing::~CameraBobbing() = default;

void
CameraBobbing::tick(Player& player, float delta_time) {
    Camera& camera = player.get_camera();

    auto&  position = camera.position();
    auto&  rotation = camera.rotation();

    float  current_rot;
    Pointf current;

    float  rot_coeff_with_easing;
    float  pos_coeff_with_easing;
    float time_coeff_with_easing;

    float (*easing_func)(float);

    if (player.is_moving()) {
        if ( rot_coeff < 1.0f)
             rot_coeff += std::min(delta_time *  rot_in_speed, 1.0f -  rot_coeff);

        if ( pos_coeff < 1.0f)
             pos_coeff += std::min(delta_time *  pos_in_speed, 1.0f -  pos_coeff);

        if (time_coeff < 1.0f)
            time_coeff += std::min(delta_time * time_in_speed, 1.0f - time_coeff);

        easing_type = true;
    }

    if (!player.is_moving()) {
        if (rot_coeff > 0.0f)
            rot_coeff  -= std::min(delta_time *  rot_out_speed,  rot_coeff);

        if (  pos_coeff > 0.0f)
            pos_coeff  -= std::min(delta_time *  pos_out_speed,  pos_coeff);

        if ( time_coeff > 0.0f)
            time_coeff -= std::min(delta_time * time_out_speed, time_coeff);

        easing_type = false;
    }

    if (easing_type) {
        easing_func = &ease_out_cubic;
    } else {
        easing_func = &ease_in_cubic;
    }

     rot_coeff_with_easing = easing_func( rot_coeff);
     pos_coeff_with_easing = easing_func( pos_coeff);
    time_coeff_with_easing = easing_func(time_coeff);

    current_rot = (std::cos(time * 22.0f) + 1.0f) * 0.002f * rot_coeff_with_easing;

    current.x =           std::cos(time * 11.0f)  * 0.02f * pos_coeff_with_easing;
    current.y = -std::abs(std::sin(time * 11.0f)) * 0.02f * pos_coeff_with_easing;

    rotation.set_pitch(rotation.pitch() - last_pitch + current_rot);

    position.x += current.x * std::cos(rotation.yaw());
    position.y += current.y;
    position.z += current.x * std::sin(rotation.yaw());

    last_pitch = current_rot;

    time += delta_time * time_coeff_with_easing;
}

float
CameraBobbing::ease_out_cubic(float x) {
    return 1.0f - ease_in_cubic(1.0f - x);
}

float
CameraBobbing::ease_in_cubic(float x) {
    return x * x * x;
}

}


