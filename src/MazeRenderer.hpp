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
#include "Point2.hpp"

namespace mazemaze {

class Maze;
class Game;

class MazeRenderer : public ITickable<Game&> {
public:
    explicit MazeRenderer(Game& game);
    virtual ~MazeRenderer() = 0;

    void enable();
    void disable();
    void tick(Game& game, float delta_time) override;
    void render();
    virtual void render_sky() = 0;

protected:
    int* visible;
    Maze& maze;
    bool* compiled;
    unsigned int draw_list;
    bool deleted;

    virtual void set_states();
    virtual void on_enable();
    virtual void on_disable();
    virtual void on_tick(float delta_time) = 0;
    virtual void enable_chunk(int num);
    virtual void compile_chunk(int num) = 0;
    virtual void render_chunks(int chunks[]);

private:
    Point2i old_hcp;
};

}
