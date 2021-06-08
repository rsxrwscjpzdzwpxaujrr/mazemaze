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
        camera_bobbing(nullptr),
        move_vector_x(0.0f),
        move_vector_z(0.0f),
        x(x), y(y), z(z),
        speed(3.0f),
        height(0.65f),
        width(0.05f) {
    camera.set_y(y + height);
}

Player::~Player() {
    if (camera_bobbing) {
        delete camera_bobbing;
        camera_bobbing = nullptr;
    }
};

void
Player::start(Maze& maze) {
    x = maze.get_start_x() + 0.5f;
    z = maze.get_start_y() + 0.5f;

    camera.set_x(x);
    camera.set_y(y + height);
    camera.set_z(z);
}

void
Player::tick(Game& game, float delta_time) {
    if (game.is_paused() || game.is_won())
        return;

    float M_PI_2f = static_cast<float>(M_PI_2);

    float pitch = camera.get_pitch();
    float yaw   = camera.get_yaw();

    move_vector_x = 0.0f;
    move_vector_z = 0.0f;

    Settings& settings = game.get_settings();

    bool move[] {sf::Keyboard::isKeyPressed(settings.get_key("right")),
                 sf::Keyboard::isKeyPressed(settings.get_key("down")),
                 sf::Keyboard::isKeyPressed(settings.get_key("left")),
                 sf::Keyboard::isKeyPressed(settings.get_key("up"))};

    {
        int i;
        float j;

        for (i = 0, j = yaw; i < 4; i++, j += M_PI_2f)
            if (move[i])
                sum_vector(j, move_vector_x, move_vector_z);
    }

    if ((move[0] ^ move[2]) && (move[1] ^ move[3])) {
        float factor = 1.0f / std::sqrt((move_vector_x * move_vector_x) +
                                        (move_vector_z * move_vector_z));

        move_vector_x *= factor;
        move_vector_z *= factor;
    }

    if (move_vector_x != 0.0f || move_vector_z != 0.0f) {
        float newx = x + move_vector_x * speed * delta_time;
        float newz = z + move_vector_z * speed * delta_time;

        try_move(game.get_maze(), newx, y, newz);
    }

    sf::Window& window = GraphicEngine::inst().get_window();

    sf::Vector2u window_half_size = window.getSize();
    window_half_size.x /= 2;
    window_half_size.y /= 2;

    sf::Vector2i cursor = sf::Mouse::getPosition(window);
    sf::Mouse::setPosition(sf::Vector2i(window_half_size), window);

    float new_pitch = pitch;
    float sensitivity = settings.get_sensitivity();

    new_pitch += (cursor.y - static_cast<int>(window_half_size.y)) * sensitivity;

    if (new_pitch > -M_PI_2f && new_pitch < M_PI_2f)
        pitch = new_pitch;

    yaw   += (cursor.x - static_cast<int>(window_half_size.x)) * sensitivity;

    camera.set_pitch(pitch);
    camera.set_yaw(yaw);

    camera.set_x(x);
    camera.set_y(y + height);
    camera.set_z(z);

    setup_camera_bobbing(settings);

    tickable_handler.tick(*this, delta_time);
}

Camera&
Player::get_camera() {
    return camera;
}

bool
Player::is_moving() const {
    return move_vector_x != 0.0f || move_vector_z != 0.0f;
}

float
Player::get_x() const {
    return x;
}

float
Player::get_y() const {
    return y;
}

float
Player::get_z() const {
    return z;
}

void
Player::set_x(float x) {
    Player::x = x;
}

void
Player::set_y(float y) {
    Player::y = y;
}

void
Player::set_z(float z) {
    Player::z = z;
}

void
Player::try_move(Maze& maze, float x, float y, float z) {
    if (!(y < 0.0f && Player::y > 0.0f))
        Player::y = y;

    if (y >= 0.0f && y <= 1.0f) {
        if (!check_collision(maze, x, Player::z))
            Player::x = x;

        if (!check_collision(maze, Player::x, z))
            Player::z = z;
    } else {
        Player::x = x;
        Player::z = z;
    }
}

bool
Player::check_collision(Maze& maze, float x, float y) {
    bool intersects = false;

    for (int i = static_cast<int>(x) - 1; i <= x + 1; i++)
        for (int j = static_cast<int>(y) - 1; j <= y + 1; j++)
            if (!maze.get_opened(i, j)) {
                float delta_x = x - std::max<float>(i, std::min<float>(x, i + 1.0f));
                float delta_y = y - std::max<float>(j, std::min<float>(y, j + 1.0f));

                intersects |= (delta_x * delta_x + delta_y * delta_y) < (width * width);
            }

    return intersects;
}

void
Player::sum_vector(float angle, float& vecX, float& vecY) {
    vecX += cosf(angle);
    vecY += sinf(angle);
}

void
Player::setup_camera_bobbing(Settings& settings) {
    if (settings.get_camera_bobbing() && !camera_bobbing) {
        camera_bobbing = new CameraBobbing;
        tickable_handler.addTickable(camera_bobbing);
    }

    else if (!settings.get_camera_bobbing() && camera_bobbing) {
        tickable_handler.removeTickable(camera_bobbing);

        delete camera_bobbing;
        camera_bobbing = nullptr;
    }
}

}
