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

#pragma once

#include "../MazeRenderer.hpp"
#include "../Skybox.hpp"

namespace mazemaze {

class Game;

namespace renderers {

class Gray : public MazeRenderer {
public:
    explicit Gray(Game& game);
    ~Gray() override;

private:
    Game& game;
    Skybox skybox;

    void set_states() override;
    void on_disable() override;
    void compile_chunk(int num) override;
    void on_tick(float delta_time) override;
    void render_chunks(int chunks[]) override;
    void render_sky() override;
};

}
}
