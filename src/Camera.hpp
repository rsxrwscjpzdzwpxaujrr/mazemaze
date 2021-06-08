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

namespace mazemaze {

class Camera {
public:
    explicit Camera(float  x,     float  y,         float  z,
                    float  pitch, float  yaw,       float  roll,
                    double fov,   double near_dist, double far_dist);
    ~Camera();

    void setup_rotation();
    void setup_translation();
    void setup_perspective();

    float get_x() const;
    float get_y() const;
    float get_z() const;

    float get_pitch() const;
    float get_yaw() const;
    float get_roll() const;

    double get_fov() const;

    void set_x(float x);
    void set_y(float y);
    void set_z(float z);

    void set_pitch(float pitch);
    void set_yaw(float yaw);
    void set_roll(float roll);

    void set_fov(double fov);

private:
    float x;
    float y;
    float z;

    float pitch;
    float yaw;
    float roll;

    double fov;
    double near_dist;
    double far_dist;
};

}
