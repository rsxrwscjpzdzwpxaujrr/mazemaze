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

#include "Player.hpp"

#include <cmath>
#include <iostream>

#include <SFML/Window.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/OpenGL.hpp>

#include "Maze.hpp"

namespace mazemaze {

Player::Player(float x, float y, float z) :
        camera(x,     0.0f,  z,
               0.0f,  0.0f,  0.0f,
               100.0, 0.001, 100.0),
        x(x), y(y), z(z) {
    camera.setY(y + height);
}

Player::~Player() = default;

void
Player::tick(float deltaTime, sf::Window* window, Maze* maze) {
    float mouseSensitivity = 0.001f;

    int intx = static_cast<int>(x);
    int intz = static_cast<int>(z);

    float M_PIf   = static_cast<float>(M_PI);
    float M_PI_2f = static_cast<float>(M_PI_2);

    float pitch = camera.getPitch();
    float yaw   = camera.getYaw();

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
    yaw   += (cursor.x - static_cast<int>(windowHalfSize.x)) * mouseSensitivity;

    camera.setPitch(pitch);
    camera.setYaw(yaw);

    camera.setX(x);
    camera.setY(y + height);
    camera.setZ(z);

    sf::Mouse::setPosition(sf::Vector2i(windowHalfSize), *window);
}

Camera*
Player::getCamera() {
    return &camera;
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

void
Player::setX(float x) {
    Player::x = x;
}

void
Player::setY(float y) {
    Player::y = y;
}

void
Player::setZ(float z) {
    Player::z = z;
}

}
