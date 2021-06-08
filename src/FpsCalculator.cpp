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

#include "FpsCalculator.hpp"

namespace mazemaze {

FpsCalculator::FpsCalculator(std::function<void (float)> const& on_update, float update_interval) :
        on_update(on_update),
        update_interval(update_interval),
        time_passed(0.0f),
        delta_sum(0.0f),
        delta_count(0.0f),
        last_fps(0.0f) {}

FpsCalculator::~FpsCalculator() = default;

void
FpsCalculator::tick(void*, float delta_time) {
    time_passed += delta_time;
    delta_sum += delta_time;
    delta_count++;

    if (time_passed >= update_interval) {
        float big_count = static_cast<int>(time_passed / update_interval);
        time_passed -= update_interval * big_count;

        last_fps = 1.0f / (delta_sum / delta_count);

        on_update(last_fps);

        delta_count = 0.0f;
        delta_sum = 0.0;
    }
}

float
FpsCalculator::get_last_fps() const {
    return last_fps;
}

float
FpsCalculator::get_update_interval() const {
    return update_interval;
}

void
FpsCalculator::set_update_interval(float update_interval) {
    FpsCalculator::update_interval = update_interval;
}

}
