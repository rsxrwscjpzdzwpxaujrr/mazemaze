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

CameraBobbing::CameraBobbing() :
    pitch_coeff(0.0f),
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

    float pitch = camera.get_pitch();
    float yaw   = camera.get_yaw();

    float x = camera.get_x();
    float y = camera.get_y();
    float z = camera.get_z();

    float current_pitch;
    float current_x;
    float current_y;

    float pitch_coeff_with_easing;
    float pos_coeff_with_easing;
    float time_coeff_with_easing;

    float (*easing_func)(float);

    if (player.is_moving()) {
        if (pitch_coeff < 1.0f)
            pitch_coeff += std::min(delta_time * 5.0f, 1.0f - pitch_coeff);

        if (  pos_coeff < 1.0f)
            pos_coeff   += std::min(delta_time * 5.0f, 1.0f -   pos_coeff);

        if ( time_coeff < 1.0f)
            time_coeff  += std::min(delta_time * 5.0f, 1.0f -  time_coeff);

        easing_type = true;
    }

    if (!player.is_moving()) {
        if (pitch_coeff > 0.0f)
            pitch_coeff -= std::min(delta_time * 2.0f, pitch_coeff);

        if (  pos_coeff > 0.0f)
            pos_coeff   -= std::min(delta_time * 3.0f,   pos_coeff);

        if ( time_coeff > 0.0f)
            time_coeff  -= std::min(delta_time * 3.0f,  time_coeff);

        easing_type = false;
    }

    if (easing_type) {
        easing_func = &ease_out_cubic;
    } else {
        easing_func = &ease_in_cubic;
    }

    pitch_coeff_with_easing = easing_func(pitch_coeff);
      pos_coeff_with_easing = easing_func(  pos_coeff);
     time_coeff_with_easing = easing_func( time_coeff);

    current_pitch = (std::cos(time * 22.0f) + 1.0f) * 0.002f * pitch_coeff_with_easing;

    current_x =           std::cos(time * 11.0f)  * 0.02f * pos_coeff_with_easing;
    current_y = -std::abs(std::sin(time * 11.0f)) * 0.02f * pos_coeff_with_easing;

    camera.set_pitch(pitch - last_pitch + current_pitch);

    camera.set_x(x + current_x * std::cos(yaw));
    camera.set_y(y + current_y);
    camera.set_z(z + current_x * std::sin(yaw));

    last_pitch = current_pitch;

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


