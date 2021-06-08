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

#pragma once

#include "ITickable.hpp"

namespace mazemaze {

class Player;

class CameraBobbing : public ITickable<Player&> {
public:
    explicit CameraBobbing();
    ~CameraBobbing();

    void tick(Player& player, float delta_time);

private:
    float pitch_coeff;
    float pos_coeff;
    float time_coeff;

    float last_pitch;

    bool easing_type;

    float time;

    static float ease_in_cubic (float x);
    static float ease_out_cubic(float x);
};

}
