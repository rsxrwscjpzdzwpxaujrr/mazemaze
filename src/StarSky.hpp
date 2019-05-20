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

#pragma once

#include "IRenderable.hpp"
#include "ITickable.hpp"

namespace mazemaze {

class StarSky : public IRenderable, public ITickable {
public:
    explicit StarSky(int starCount, float timeSpeed, float pitch, float yaw);
    ~StarSky() override;

    void generate();

    void render() override;
    void tick(float deltatime) override;
    void setTime(float time);

private:
    bool firstGeneration = true;
    int starCount;
    float* starsX;
    float* starsY;
    float* starsZ;
    int* starSize;

    float distance = 90.0f;

    float pitch;
    float yaw;
    float roll;

    float time;
    float timeSpeed;

    unsigned int drawList;
};

}
