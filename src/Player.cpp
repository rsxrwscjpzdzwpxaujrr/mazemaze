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

Player::Player(Pointf position) :
        camera(position,
               Rotation(),
               100.0, 0.005, 100.0),
        camera_bobbing(nullptr),
        m_position(position),
        speed(3.0f),
        height(0.65f),
        width(0.05f) {
    camera.position().y = position.y + height;
}

Player::~Player() {
    if (camera_bobbing) {
        delete camera_bobbing;
        camera_bobbing = nullptr;
    }
};

void
Player::start(Maze& maze) {
    Pointf& cam_pos = camera.position();

    m_position.x = maze.start().x + 0.5f;
    m_position.z = maze.start().y + 0.5f;

    cam_pos.set(Pointf(m_position.x, m_position.y + height, m_position.z));
}

void
Player::tick(Game& game, float delta_time) {
    if (game.is_paused() || game.is_won())
        return;

    float M_PI_2f = static_cast<float>(M_PI_2);

    auto& rotation = camera.rotation();
    auto& cam_pos  = camera.position();

    m_move_vector.x = 0.0f;
    m_move_vector.y = 0.0f;

    Settings& settings = game.get_settings();

    bool move[] {
        sf::Keyboard::isKeyPressed(settings.get_key("right")),
        sf::Keyboard::isKeyPressed(settings.get_key("down")),
        sf::Keyboard::isKeyPressed(settings.get_key("left")),
        sf::Keyboard::isKeyPressed(settings.get_key("up"))
    };

    for (int i = 0, j = rotation.yaw(); i < 4; i++, j += M_PI_2f)
        if (move[i])
            sum_vector(j, m_move_vector);

    if ((move[0] ^ move[2]) && (move[1] ^ move[3])) {
        float factor = 1.0f / std::sqrt((m_move_vector.x * m_move_vector.x) +
                                        (m_move_vector.y * m_move_vector.y));

        m_move_vector.x *= factor;
        m_move_vector.y *= factor;
    }

    if (m_move_vector.x != 0.0f || m_move_vector.y != 0.0f) {
        float newx = m_position.x + m_move_vector.x * speed * delta_time;
        float newz = m_position.z + m_move_vector.y * speed * delta_time;

        try_move(game.get_maze(), Pointf(newx, m_position.y, newz));
    }

    sf::Window& window = GraphicEngine::inst().get_window();

    sf::Vector2u window_half_size = window.getSize();
    window_half_size.x /= 2;
    window_half_size.y /= 2;

    sf::Vector2i cursor = sf::Mouse::getPosition(window);
    sf::Mouse::setPosition(sf::Vector2i(window_half_size), window);

    float new_pitch = rotation.pitch();
    float sensitivity = settings.get_sensitivity();

    new_pitch += (cursor.y - static_cast<int>(window_half_size.y)) * sensitivity;

    if (new_pitch > -M_PI_2f && new_pitch < M_PI_2f)
        rotation.set_pitch(new_pitch);

    rotation.set_yaw(
        rotation.yaw() +
        (cursor.x - static_cast<int>(window_half_size.x)) * sensitivity
    );

    cam_pos.set(Pointf(m_position.x, m_position.y + height, m_position.z));

    setup_camera_bobbing(settings);

    tickable_handler.tick(*this, delta_time);
}

Camera&
Player::get_camera() {
    return camera;
}

bool
Player::is_moving() const {
    return m_move_vector.x != 0.0f || m_move_vector.y != 0.0f;
}

Pointf&
Player::position() {
    return m_position;
}

void
Player::try_move(Maze& maze, Pointf pos) {
    auto& m_pos = m_position;

    if (!(pos.y < 0.0f && m_pos.y > 0.0f))
        m_pos.y = pos.y;

    if (pos.y >= 0.0f && pos.y <= 1.0f) {
        if (!check_collision(maze, Pointf(pos.x, 0.0f, m_pos.z)))
            m_pos.x = pos.x;

        if (!check_collision(maze, Pointf(m_pos.x, 0.0f, pos.z)))
            m_pos.z = pos.z;
    } else {
        m_pos.set(pos);
    }
}

bool
Player::check_collision(Maze& maze, Pointf pos) {
    using namespace std;

    bool intersects = false;

    for (int i = static_cast<int>(pos.x) - 1; i <= pos.x + 1; i++)
        for (int j = static_cast<int>(pos.z) - 1; j <= pos.z + 1; j++)
            if (!maze.get_opened(Point2i(i, j))) {
                Point2f delta(
                    pos.x - max<float>(i, min<float>(pos.x, i + 1.0f)),
                    pos.z - max<float>(j, min<float>(pos.z, j + 1.0f))
                );

                intersects |= (delta.x * delta.x + delta.y * delta.y) < (width * width);
            }

    return intersects;
}

void
Player::sum_vector(float angle, Point2f& vec) {
    vec.x += cosf(angle);
    vec.y += sinf(angle);
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
