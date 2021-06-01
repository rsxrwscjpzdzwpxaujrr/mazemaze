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

FpsCalculator::FpsCalculator(std::function<void (float)> const& onUpdate, float updateInterval) :
        onUpdate(onUpdate),
        updateInterval(updateInterval),
        timePassed(0.0f),
        deltaSum(0.0f),
        deltaCount(0.0f),
        lastFps(0.0f) {}

FpsCalculator::~FpsCalculator() = default;

void
FpsCalculator::tick(void*, float deltaTime) {
    timePassed += deltaTime;
    deltaSum += deltaTime;
    deltaCount++;

    if (timePassed >= updateInterval) {
        float bigCount = static_cast<int>(timePassed / updateInterval);
        timePassed -= updateInterval * bigCount;

        lastFps = 1.0f / (deltaSum / deltaCount);

        onUpdate(lastFps);

        deltaCount = 0.0f;
        deltaSum = 0.0;
    }
}

float
FpsCalculator::getLastFps() const {
    return lastFps;
}

float
FpsCalculator::getUpdateInterval() const {
    return updateInterval;
}

void
FpsCalculator::setUpdateInterval(float updateInterval) {
    FpsCalculator::updateInterval = updateInterval;
}

}
