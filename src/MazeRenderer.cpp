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

#include "MazeRenderer.hpp"

#include <functional>

#include <SFML/OpenGL.hpp>

#include "GraphicEngine.hpp"
#include "Chunk.hpp"
#include "Game.hpp"
#include "Logger.hpp"

namespace mazemaze {

MazeRenderer::MazeRenderer(Game& game) : maze(game.get_maze()),
                                         deleted(true),
                                         old_hcp_x(-1),
                                         old_hcp_y(-1) {}

MazeRenderer::~MazeRenderer() {
    if (!deleted)
        disable();
}

void
MazeRenderer::enable() {
    visible  = new int[16] {-1};
    compiled = new bool[maze.get_chunks_count()] {false};
    draw_list = glGenLists(maze.get_chunks_count());

    set_states();
    on_enable();

    GraphicEngine::inst().set_on_set_states_callback([this] {
        this->set_states();
    });

    deleted = false;
}

void
MazeRenderer::disable() {
    GraphicEngine::inst().set_on_set_states_callback([] {});

    on_disable();

    glDeleteLists(draw_list, maze.get_chunks_count());

    delete [] visible;
    delete [] compiled;

    deleted = true;
}

void
MazeRenderer::tick(Game& game, float deltaTime) {
    bool force = false;

    Player& player = game.get_player();

    int p_x = static_cast<int>(player.get_x()) / (Chunk::SIZE / 2);
    int p_y = static_cast<int>(player.get_z()) / (Chunk::SIZE / 2);

    if (p_x != old_hcp_x || p_y != old_hcp_y || force) {
        Logger::inst().log_debug("Re-enabling chunks.");

        old_hcp_x = p_x;
        old_hcp_y = p_y;

        for (int i = 0; i < 16; i++)
            visible[i] = -1;

        if (p_x % 2 == 0) p_x--;
        if (p_y % 2 == 0) p_y--;

        p_x /= 2;
        p_y /= 2;

        int pe_x = p_x + 2;
        int pe_y = p_y + 2;

        if (p_x < 0) p_x = 0;
        if (p_y < 0) p_y = 0;

        if (pe_x > maze.get_chunks_x()) pe_x = maze.get_chunks_x();
        if (pe_y > maze.get_chunks_y()) pe_y = maze.get_chunks_y();

        for (int i = p_x; i < pe_x; i++)
            for (int j = p_y; j < pe_y; j++) {
                int chunkNum = i + j * maze.get_chunks_x();

                enable_chunk(chunkNum);
            }
    }

    on_tick(deltaTime);
}

void
MazeRenderer::render() {
    int chunks[16 + 1];
    int index = 0;

    chunks[16] = -1;

    for (int i = 0; i < 16; i++) {
        if (visible[i] != -1) {
            chunks[index] = visible[i];
            index++;
        } else {
            chunks[index] = -1;
            break;
        }
    }

    render_chunks(chunks);
}

void
MazeRenderer::set_states() {
}

void
MazeRenderer::on_enable() {
}

void
MazeRenderer::on_disable() {
}

void
MazeRenderer::enable_chunk(int num) {
    if (!compiled[num])
        compile_chunk(num);

    for (int i = 0; i < 16; i++) {
        if (visible[i] == -1) {
            visible[i] = num;
            break;
        }
    }
}

void
MazeRenderer::render_chunks(int chunks[]) {
    for (; *chunks != -1; chunks++)
        glCallList(draw_list + *chunks);
}

}
