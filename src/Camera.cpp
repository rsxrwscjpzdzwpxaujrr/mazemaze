/*
 * Copyright (c) 2019, Мира Странная <rsxrwscjpzdzwpxaujrr@yahoo.com>
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

Camera::Camera(float  x,     float  y,        float  z,
               float  pitch, float  yaw,      float  roll,
               double fov,   double nearDist, double farDist) :
        x(x),         y(y),               z(z),
        pitch(pitch), yaw(yaw),           roll(roll),
        fov(fov),     nearDist(nearDist), farDist(farDist) {}

Camera::~Camera() = default;

void
Camera::setupRotation() {
    float todeg = static_cast<float>(180.0 / M_PI);

    glRotatef(pitch * todeg, 1.0, 0.0, 0.0);
    glRotatef(yaw * todeg, 0.0, 1.0, 0.0);
    glRotatef(roll * todeg, 0.0, 0.0, 1.0);
}

void
Camera::setupTranslation() {
    glTranslatef(-x, -y, -z);
}

void
Camera::setupPerspective() {
    double ratio = GraphicEngine::getInstance().getWidth() /
                   static_cast<double>(GraphicEngine::getInstance().getHeight());

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glFrustum(-ratio * nearDist, ratio * nearDist,
              -1.0   * nearDist, 1.0   * nearDist,
              (ratio * nearDist) / tan(fov * (M_PI / 360.0)),
              farDist);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

float
Camera::getX() const {
    return x;
}

float
Camera::getY() const {
    return y;
}

float
Camera::getZ() const {
    return z;
}

float
Camera::getPitch() const {
    return pitch;
}

float
Camera::getYaw() const {
    return yaw;
}

float
Camera::getRoll() const {
    return roll;
}

double
Camera::getFov() const {
    return fov;
}

void
Camera::setX(float x) {
    Camera::x = x;
}

void
Camera::setY(float y) {
    Camera::y = y;
}

void
Camera::setZ(float z) {
    Camera::z = z;
}

void
Camera::setPitch(float pitch) {
    Camera::pitch = pitch;
}

void
Camera::setYaw(float yaw) {
    Camera::yaw = yaw;
}

void
Camera::setRoll(float roll) {
    Camera::roll = roll;
}

void
Camera::setFov(double fov) {
    Camera::fov = fov;
}

}
