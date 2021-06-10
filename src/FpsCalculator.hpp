/*
 * Copyright (c) 2019-2021, Мира Странная <rsxrwscjpzdzwpxaujrr@yahoo.com>
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

#include <functional>

#include "ITickable.hpp"

namespace mazemaze {

class FpsCalculator : public ITickable<void*> {
public:
    explicit FpsCalculator(std::function<void (float)> const& on_update, float update_interval);
    ~FpsCalculator() override;

    void tick(void*, float delta_time) override;

    void set_update_interval(float update_interval);

    float last_fps() const;
    float update_interval() const;

private:
    std::function<void (float)> on_update;
    float m_update_interval;
    float time_passed;

    float delta_sum;
    float delta_count;
    float m_last_fps;
};

}
