/*
 * Copyright (c) 2019, Мира Странная <miraityan2004@gmail.com>
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

#include "../ITickable.hpp"
#include "../IRenderable.hpp"

namespace mazemaze {

class Camera;

namespace gui {

class Background : public ITickable, public IRenderable {
public:
    explicit Background(ITickable* tickable, IRenderable* renderable, Camera* camera);
    ~Background() override;

    void tick(float deltaTime) override;
    void render() override;

    ITickable*      getTickable() const;
    IRenderable*    getRenderable() const;
    virtual Camera* getCamera();

private:
    ITickable* tickable;
    IRenderable* renderable;
    Camera* camera;
};

}
}
