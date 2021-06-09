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
#include "Rotation.hpp"

namespace mazemaze {

class StarSky : public IRenderable, public ITickable<void*> {
public:
    explicit StarSky(int star_count, float time_speed, Rotation rotation);
    ~StarSky() override;

    void generate();

    void render() override;
    void tick(void*, float delta_time) override;
    void set_time(float time);

private:
    struct Star {
        float x;
        float y;
        float z;
        int size;
    };

    int star_count;

    float distance;

    Rotation m_rotation;

    float time;
    float time_speed;

    unsigned int draw_list;

    std::vector<Star> stars;

    void compile();
};

}
