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

Camera::Camera(float  x,     float  y,         float  z,
               float  pitch, float  yaw,       float  roll,
               double fov,   double near_dist, double far_dist) :
        x(x),         y(y),                z(z),
        pitch(pitch), yaw(yaw),            roll(roll),
        fov(fov),     near_dist(near_dist), far_dist(far_dist) {}

Camera::~Camera() = default;

void
Camera::setup_rotation() {
    float todeg = static_cast<float>(180.0 / M_PI);

    glRotatef(pitch * todeg, 1.0, 0.0, 0.0);
    glRotatef(yaw   * todeg, 0.0, 1.0, 0.0);
    glRotatef(roll  * todeg, 0.0, 0.0, 1.0);
}

void
Camera::setup_translation() {
    glTranslatef(-x, -y, -z);
}

void
Camera::setup_perspective() {
    double ratio = GraphicEngine::inst().get_width() /
                   static_cast<double>(GraphicEngine::inst().get_height());

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glFrustum(-ratio * near_dist, ratio * near_dist,
              -1.0   * near_dist, 1.0   * near_dist,
              (ratio * near_dist) / tan(fov * (M_PI / 360.0)),
              far_dist);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

float
Camera::get_x() const {
    return x;
}

float
Camera::get_y() const {
    return y;
}

float
Camera::get_z() const {
    return z;
}

float
Camera::get_pitch() const {
    return pitch;
}

float
Camera::get_yaw() const {
    return yaw;
}

float
Camera::get_roll() const {
    return roll;
}

double
Camera::get_fov() const {
    return fov;
}

void
Camera::set_x(float x) {
    Camera::x = x;
}

void
Camera::set_y(float y) {
    Camera::y = y;
}

void
Camera::set_z(float z) {
    Camera::z = z;
}

void
Camera::set_pitch(float pitch) {
    Camera::pitch = pitch;
}

void
Camera::set_yaw(float yaw) {
    Camera::yaw = yaw;
}

void
Camera::set_roll(float roll) {
    Camera::roll = roll;
}

void
Camera::set_fov(double fov) {
    Camera::fov = fov;
}

}
