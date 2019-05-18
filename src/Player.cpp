/*
 * Copyright (c) 2018-2019, Мира Странная <miraityan2004@gmail.com>
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

#include <cmath>
#include <iostream>

#include <SFML/Window.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/OpenGL.hpp>

#include "Maze.hpp"

#include "Player.hpp"

namespace mazemaze {

Player::Player(float x, float y, float z) {
    Player::x = x;
    Player::y = y;
    Player::z = z;
}

Player::~Player() {

}

void
Player::tick(float deltaTime, sf::Window *window, Maze *maze) {
    float mouseSensitivity = 0.001f;

    int intx = static_cast<int>(x);
    int intz = static_cast<int>(z);

    float M_PIf = static_cast<float>(M_PI);
    float M_PI_2f = static_cast<float>(M_PI_2);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        if (maze->getOpened(static_cast<int>(x + cosf(yaw) * speed * deltaTime), intz))
            x += cosf(yaw) * speed * deltaTime;

        if (maze->getOpened(intx, static_cast<int>(z + sinf(yaw) * speed * deltaTime)))
            z += sinf(yaw) * speed * deltaTime;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        if (maze->getOpened(static_cast<int>(x + cosf(yaw + M_PI_2f) * speed * deltaTime), intz))
            x += cosf(yaw + M_PI_2f) * speed * deltaTime;

        if (maze->getOpened(intx, static_cast<int>(z + sinf(yaw + M_PI_2f) * speed * deltaTime)))
            z += sinf(yaw + M_PI_2f) * speed * deltaTime;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        if (maze->getOpened(static_cast<int>(x + cosf(yaw + M_PIf) * speed * deltaTime), intz))
            x += cosf(yaw + M_PIf) * speed * deltaTime;

        if (maze->getOpened(intx, static_cast<int>(z + sinf(yaw + M_PIf) * speed * deltaTime)))
            z += sinf(yaw + M_PIf) * speed * deltaTime;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        if (maze->getOpened(static_cast<int>(x + cosf(yaw + M_PI_2f + M_PIf) * speed * deltaTime), intz))
            x += cosf(yaw + M_PI_2f + M_PIf) * speed * deltaTime;

        if (maze->getOpened(intx, static_cast<int>(z + sinf(yaw + M_PI_2f + M_PIf) * speed * deltaTime)))
            z += sinf(yaw + M_PI_2f + M_PIf) * speed * deltaTime;
    }

    sf::Vector2u windowHalfSize = window->getSize();
    windowHalfSize.x /= 2;
    windowHalfSize.y /= 2;

    sf::Vector2i cursor = sf::Mouse::getPosition(*window);

    pitch += (cursor.y - static_cast<int>(windowHalfSize.y)) * mouseSensitivity;
    yaw += (cursor.x - static_cast<int>(windowHalfSize.x)) * mouseSensitivity;

    if (pitch < -M_PI_2f)
        pitch = -M_PI_2f;
    else if (pitch >= M_PI_2f)
        pitch = M_PI_2f;

    sf::Mouse::setPosition(sf::Vector2i(windowHalfSize), *window);
}

void
Player::setCameraRotation() {
    float todeg = static_cast<float>(180.0 / M_PI);

    glRotatef(pitch * todeg, 1.0, 0.0, 0.0);
    glRotatef(yaw * todeg, 0.0, 1.0, 0.0);
}

void
Player::setCameraTranslation() {
    glTranslatef(-x, -y, -z);
}

float
Player::getX() const {
    return x;
}

float
Player::getY() const {
    return y;
}

float
Player::getZ() const {
    return z;
}

}
