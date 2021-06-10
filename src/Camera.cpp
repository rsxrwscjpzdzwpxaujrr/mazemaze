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

#include "Camera.hpp"

#include <cmath>

#include <SFML/OpenGL.hpp>

#include "GraphicEngine.hpp"

namespace mazemaze {

Camera::Camera(Pointf position,
               Rotation rotation,
               double fov,   double near_dist, double far_dist) :
        m_position(position),
        m_rotation(rotation),
        m_fov(fov), near_dist(near_dist), far_dist(far_dist) {}

Camera::~Camera() = default;

void
Camera::setup_rotation() {
    float todeg = static_cast<float>(180.0 / M_PI);

    glRotatef(m_rotation.pitch() * todeg, 1.0, 0.0, 0.0);
    glRotatef(m_rotation.yaw()   * todeg, 0.0, 1.0, 0.0);
    glRotatef(m_rotation.roll()  * todeg, 0.0, 0.0, 1.0);
}

void
Camera::setup_translation() {
    glTranslatef(-m_position.x, -m_position.y, -m_position.z);
}

void
Camera::setup_perspective() {
    auto window_size = GraphicEngine::inst().window_size();

    double ratio = window_size.x / static_cast<double>(window_size.y);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glFrustum(-ratio * near_dist, ratio * near_dist,
              -1.0   * near_dist, 1.0   * near_dist,
              (ratio * near_dist) / tan(m_fov * (M_PI / 360.0)),
              far_dist);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

Pointf&
Camera::position() {
    return m_position;
}

Rotation&
Camera::rotation() {
    return m_rotation;
}

double
Camera::fov() const {
    return m_fov;
}

void
Camera::set_fov(double fov) {
    m_fov = fov;
}

}
