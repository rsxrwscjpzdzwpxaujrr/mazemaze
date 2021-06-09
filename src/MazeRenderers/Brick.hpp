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

protected:
    Game& game;

private:
    enum Angle {
        NO = 0,
        INNER = 1,
        OUTER = 2,
    };

    const int mesh_count;

    Skybox skybox;
    int mesh_draw_list;

    void compile_walls();
    void compile_wall(objl::Mesh& mesh, Angle angle_type, bool v_mirror, bool side);
    void draw_mortar(Angle angle_type, bool side);
    unsigned int get_mesh(Angle angle_type, bool v_mirror, bool side);

    Angle get_angle(bool openeds[]);

    void set_states() override;
    void on_enable() override;
    void on_disable() override;
    void compile_chunk(int num) override;
    void on_tick(float deltaTime) override;
    void render_chunks(int chunks[]) override;
    void render_sky() override;
    void render_wall(Angle left_angle, Angle right_angle, bool flip);
};

}
}
