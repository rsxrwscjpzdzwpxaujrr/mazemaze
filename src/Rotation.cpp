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

#include "Rotation.hpp"

#include <cmath>

using namespace mazemaze;

Rotation::Rotation(float pitch, float yaw, float roll) :
        Rotation() {
    set_pitch(pitch);
    set_yaw(yaw);
    set_roll(roll);
}

Rotation::Rotation() :
    m_pitch(0.0f),
    m_yaw  (0.0f),
    m_roll (0.0f) {
}

void
Rotation::set(Rotation rotation) {
    m_pitch = rotation.m_pitch;
    m_yaw   = rotation.m_yaw;
    m_roll  = rotation.m_roll;
}

float
Rotation::pitch() const {
    return m_pitch;
}

float
Rotation::yaw() const {
    return m_yaw;
}

float
Rotation::roll() const {
    return m_roll;
}

void
Rotation::set_pitch(float pitch) {
    m_pitch = normalize(pitch);
}

void
Rotation::set_yaw(float yaw) {
    m_yaw = normalize(yaw);
}

void
Rotation::set_roll(float roll) {
    m_roll = normalize(roll);
}

float
Rotation::normalize(float value) {
    float pi2 = M_PI * 2.0f;

    return value - (static_cast<int>(value / pi2) * pi2);
}
