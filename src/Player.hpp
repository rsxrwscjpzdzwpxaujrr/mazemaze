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
#include "Point.hpp"
#include "Point2.hpp"
#include "Rotation.hpp"

namespace mazemaze {

class Maze;
class Game;
class Settings;

class Player : public ITickable<Game&> {
public:
    explicit Player(Pointf position);
    ~Player();

    void start(Maze& maze);
    void tick(Game& game, float delta_time) override;

    Camera& get_camera();

    bool is_moving() const;

    Pointf& position();

private:
    Camera camera;
    TickableHandler<Player&> tickable_handler;
    CameraBobbing* camera_bobbing;

    Pointf  m_position;
    Point2f m_move_vector;

    float speed;
    float height;
    float width;

    void try_move(Maze& maze, Pointf position);
    bool check_collision(Maze& maze, Pointf position);
    void sum_vector(float angle, Point2f& vec);
    void setup_camera_bobbing(Settings& settings);
};

}
