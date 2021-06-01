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

#pragma once

#include <vector>

#include "IRenderable.hpp"
#include "ITickable.hpp"

namespace mazemaze {

class StarSky : public IRenderable, public ITickable<void*> {
public:
    explicit StarSky(int starCount, float timeSpeed, float pitch, float yaw);
    ~StarSky() override;

    void generate();

    void render() override;
    void tick(void*, float deltatime) override;
    void setTime(float time);

private:
    struct Star {
        float x;
        float y;
        float z;
        int size;
    };

    int starCount;

    float distance;

    float pitch;
    float yaw;
    float roll;

    float time;
    float timeSpeed;

    unsigned int drawList;

    std::vector<Star> stars;

    void compile();
};

}
