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

    void tick(Player& player, float deltaTime);

private:
    float pitchCoeff;
    float posCoeff;
    float timeCoeff;

    float lastPitch;

    bool easingType;

    float time;

    static float easeInCubic (float x);
    static float easeOutCubic(float x);
};

}
