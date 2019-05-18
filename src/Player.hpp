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

#pragma once

#include <SFML/Window.hpp>

namespace mazemaze {

class Maze;

class Player {
public:
    explicit Player(float x, float y, float z);
    ~Player();

    void tick(float deltaTime, sf::Window *window, Maze *maze);

    void setCameraRotation();
    void setCameraTranslation();

    float getX() const;
    float getY() const;
    float getZ() const;

private:
    float x;
    float y;
    float z;
    float pitch = 0.0f;
    float yaw = 0.0f;
    float roll = 0.0f;
    float speed = 3.0f;
};

}
