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
    tickable(tickable),
    renderable(renderable),
    camera(camera) {}

Background::~Background() = default;

void
Background::tick(void* _, float delta_time) {
    tickable->tick(_, delta_time);
}

void
Background::render() {
    glPushMatrix();

    camera->setup_perspective();
    camera->setup_rotation();
    camera->setup_translation();

    renderable->render();

    glPopMatrix();
}

ITickable<void*>*
Background::get_tickable() const {
    return tickable;
}

IRenderable*
Background::get_renderable() const {
    return renderable;
}

Camera*
Background::get_camera() {
    return camera;
}

}
}
