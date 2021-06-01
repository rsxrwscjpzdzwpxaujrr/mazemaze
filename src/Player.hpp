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

#pragma once

#include "ITickable.hpp"

#include <SFML/Window.hpp>

#include "Camera.hpp"
#include "CameraBobbing.hpp"
#include "TickableHandler.hpp"

namespace mazemaze {

class Maze;
class Game;
class Settings;

class Player : public ITickable<Game&> {
public:
    explicit Player(float x, float y, float z);
    ~Player();

    void start(Maze& maze);
    void tick(Game& game, float deltaTime) override;

    Camera& getCamera();

    bool isMoving() const;

    float getX() const;
    float getY() const;
    float getZ() const;

    void setX(float x);
    void setY(float y);
    void setZ(float z);

private:
    Camera camera;
    TickableHandler<Player&> tickableHandler;
    CameraBobbing* cameraBobbing;

    float moveVectorX;
    float moveVectorZ;
    float x;
    float y;
    float z;
    float speed;
    float height;
    float width;

    void tryMove(Maze& maze, float x, float y, float z);
    bool checkCollision(Maze& maze, float x, float y);
    void sumVector(float angle, float& vecX, float& vecY);
    void setupCameraBobbing(Settings& settings);
};

}
