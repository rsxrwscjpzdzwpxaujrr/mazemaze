/*
 * Copyright (c) 2018-2021, Мира Странная <rsxrwscjpzdzwpxaujrr@yahoo.com>
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
#include <algorithm>

#include <SFML/Window.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/OpenGL.hpp>

#include "Maze.hpp"
#include "Game.hpp"
#include "Settings.hpp"
#include "GraphicEngine.hpp"

namespace mazemaze {

Player::Player(float x, float y, float z) :
        camera(x,     0.0f,  z,
               0.0f,  0.0f,  0.0f,
               100.0, 0.005, 100.0),
        cameraBobbing(nullptr),
        moveVectorX(0.0f),
        moveVectorZ(0.0f),
        x(x), y(y), z(z),
        speed(3.0f),
        height(0.65f),
        width(0.05f) {
    camera.setY(y + height);
}

Player::~Player() = default;

void
Player::start(Maze& maze) {
    x = maze.getStartX() + 0.5f;
    z = maze.getStartY() + 0.5f;

    camera.setX(x);
    camera.setY(y + height);
    camera.setZ(z);
}

void
Player::tick(Game& game, float deltaTime) {
    if (game.isPaused() || game.isWon())
        return;

    float M_PI_2f = static_cast<float>(M_PI_2);

    float pitch = camera.getPitch();
    float yaw   = camera.getYaw();

    moveVectorX = 0.0f;
    moveVectorZ = 0.0f;

    Settings& settings = game.getSettings();

    bool move[] {sf::Keyboard::isKeyPressed(settings.getKey("right")),
                 sf::Keyboard::isKeyPressed(settings.getKey("down")),
                 sf::Keyboard::isKeyPressed(settings.getKey("left")),
                 sf::Keyboard::isKeyPressed(settings.getKey("up"))};

    {
        int i;
        float j;

        for (i = 0, j = yaw; i < 4; i++, j += M_PI_2f)
            if (move[i])
                sumVector(j, moveVectorX, moveVectorZ);
    }

    if ((move[0] ^ move[2]) && (move[1] ^ move[3])) {
        float factor = 1.0f / std::sqrt((moveVectorX * moveVectorX) +
                                        (moveVectorZ * moveVectorZ));

        moveVectorX *= factor;
        moveVectorZ *= factor;
    }

    if (moveVectorX != 0.0f || moveVectorZ != 0.0f) {
        float newx = x + moveVectorX * speed * deltaTime;
        float newz = z + moveVectorZ * speed * deltaTime;

        tryMove(game.getMaze(), newx, y, newz);
    }

    sf::Window& window = GraphicEngine::inst().getWindow();

    sf::Vector2u windowHalfSize = window.getSize();
    windowHalfSize.x /= 2;
    windowHalfSize.y /= 2;

    sf::Vector2i cursor = sf::Mouse::getPosition(window);
    sf::Mouse::setPosition(sf::Vector2i(windowHalfSize), window);

    float newPitch = pitch;
    float sensitivity = settings.getSensitivity();

    newPitch += (cursor.y - static_cast<int>(windowHalfSize.y)) * sensitivity;

    if (newPitch > -M_PI_2f && newPitch < M_PI_2f)
        pitch = newPitch;

    yaw   += (cursor.x - static_cast<int>(windowHalfSize.x)) * sensitivity;

    camera.setPitch(pitch);
    camera.setYaw(yaw);

    camera.setX(x);
    camera.setY(y + height);
    camera.setZ(z);

    setupCameraBobbing(settings);

    tickableHandler.tick(*this, deltaTime);
}

Camera&
Player::getCamera() {
    return camera;
}

bool
Player::isMoving() const {
    return moveVectorX != 0.0f || moveVectorZ != 0.0f;
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

void
Player::tryMove(Maze& maze, float x, float y, float z) {
    if (!(y < 0.0f && Player::y > 0.0f))
        Player::y = y;

    if (y >= 0.0f && y <= 1.0f) {
        if (!checkCollision(maze, x, Player::z))
            Player::x = x;

        if (!checkCollision(maze, Player::x, z))
            Player::z = z;
    } else {
        Player::x = x;
        Player::z = z;
    }
}

bool
Player::checkCollision(Maze& maze, float x, float y) {
    bool intersects = false;

    for (int i = static_cast<int>(x) - 1; i <= x + 1; i++)
        for (int j = static_cast<int>(y) - 1; j <= y + 1; j++)
            if (!maze.getOpened(i, j)) {
                float deltaX = x - std::max<float>(i, std::min<float>(x, i + 1.0f));
                float deltaY = y - std::max<float>(j, std::min<float>(y, j + 1.0f));

                intersects |= (deltaX * deltaX + deltaY * deltaY) < (width * width);
            }

    return intersects;
}

void
Player::sumVector(float angle, float& vecX, float& vecY) {
    vecX += cosf(angle);
    vecY += sinf(angle);
}

void
Player::setupCameraBobbing(Settings& settings) {
    if (settings.getCameraBobbing() && !cameraBobbing) {
        cameraBobbing = new CameraBobbing;
        tickableHandler.addTickable(cameraBobbing);
    }

    else if (!settings.getCameraBobbing() && cameraBobbing) {
        tickableHandler.removeTickable(cameraBobbing);
        cameraBobbing = nullptr;
    }
}

}
