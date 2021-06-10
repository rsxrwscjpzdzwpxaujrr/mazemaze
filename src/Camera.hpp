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

#include "Point.hpp"
#include "Rotation.hpp"

namespace mazemaze {

class Camera {
public:
    explicit Camera(Pointf position, Rotation rotation,
                    double fov, double near_dist, double far_dist);
    ~Camera();

    void setup_rotation();
    void setup_translation();
    void setup_perspective();

    Pointf  & position();
    Rotation& rotation();

    double fov() const;

    void set_fov(double fov);

private:
    Pointf   m_position;
    Rotation m_rotation;

    double m_fov;
    double near_dist;
    double far_dist;
};

}
