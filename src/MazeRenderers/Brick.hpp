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

#include "../MazeRenderer.hpp"
#include "../Skybox.hpp"

namespace objl {

class Mesh;

}

namespace mazemaze {

class Game;

namespace renderers {

class Brick : public MazeRenderer {
public:
    explicit Brick(Game& game);
    ~Brick() override;

private:
    enum Angle {
        NO = 0,
        INNER = 1,
        OUTER = 2,
    };

    const int meshCount;

    Game& game;
    Skybox skybox;
    int meshDrawList;

    void compileWalls();
    void compileWall(objl::Mesh& mesh, Angle angleType, bool vMirror, bool side);
    void drawMortar(Angle angleType, bool side);
    unsigned int getMesh(Angle angleType, bool vMirror, bool side);

    Angle getAngle(bool openeds[]);

    void setStates() override;
    void onEnable() override;
    void onDisable() override;
    void compileChunk(int num) override;
    void onTick(float deltaTime) override;
    void renderChunks(int chunks[]) override;
    void renderWall(Angle leftAngle, Angle rightAngle, bool flip);
};

}
}
