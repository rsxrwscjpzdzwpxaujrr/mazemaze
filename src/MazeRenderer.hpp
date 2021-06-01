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

namespace mazemaze {

class Maze;
class Game;

class MazeRenderer : public ITickable<Game&> {
public:
    explicit MazeRenderer(Game& game);
    virtual ~MazeRenderer() = 0;

    void enable();
    void disable();
    void tick(Game& game, float deltaTime) override;
    void render();

protected:
    int* visible;
    Maze& maze;
    bool* compiled;
    unsigned int drawList;
    bool deleted;

    virtual void setStates();
    virtual void onEnable();
    virtual void onDisable();
    virtual void onTick(float deltaTime) = 0;
    virtual void enableChunk(int num);
    virtual void compileChunk(int num) = 0;
    virtual void renderChunks(int chunks[]);

private:
    int oldHcpX;
    int oldHcpY;
};

}
