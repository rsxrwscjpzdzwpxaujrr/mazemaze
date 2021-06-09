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

namespace mazemaze {

class Rotation {
public:
    explicit Rotation(float pitch, float yaw, float roll);
    Rotation();

    void set(Rotation rotation);

    float pitch() const;
    float yaw  () const;
    float roll () const;

    void set_pitch(float pitch);
    void set_yaw  (float yaw  );
    void set_roll (float roll );

private:
    float m_pitch;
    float m_yaw;
    float m_roll;

    float normalize(float value);
};

}
