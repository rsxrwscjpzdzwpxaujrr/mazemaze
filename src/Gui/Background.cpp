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

#include "Background.hpp"

#include "SFML/OpenGL.hpp"

#include "../Camera.hpp"

namespace mazemaze {
namespace gui {

Background::Background(ITickable<void*>* tickable, IRenderable* renderable, Camera* camera) :
    m_tickable(tickable),
    m_renderable(renderable),
    m_camera(camera) {}

Background::~Background() = default;

void
Background::tick(void* _, float delta_time) {
    m_tickable->tick(_, delta_time);
}

void
Background::render() {
    glPushMatrix();

    m_camera->setup_perspective();
    m_camera->setup_rotation();
    m_camera->setup_translation();

    m_renderable->render();

    glPopMatrix();
}

ITickable<void*>*
Background::tickable() const {
    return m_tickable;
}

IRenderable*
Background::renderable() const {
    return m_renderable;
}

Camera*
Background::camera() {
    return m_camera;
}

}
}
