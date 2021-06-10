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

MazeRenderer::MazeRenderer(Game& game) : maze(game.maze()),
                                         deleted(true),
                                         old_hcp(-1, -1) {}

MazeRenderer::~MazeRenderer() {
    if (!deleted)
        disable();
}

void
MazeRenderer::enable() {
    int chunks_count = maze.chunks_count().x * maze.chunks_count().y;

    visible  = new int[16] {-1};
    compiled = new bool[chunks_count] {false};
    draw_list = glGenLists(chunks_count);

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

    int chunk_count = maze.chunks_count().x * maze.chunks_count().y;

    glDeleteLists(draw_list, chunk_count);

    delete [] visible;
    delete [] compiled;

    deleted = true;
}

void
MazeRenderer::tick(Game& game, float delta_time) {
    bool force = false;

    Player& player = game.player();

    Point2i p(
        static_cast<int>(player.position().x) / (Chunk::SIZE / 2),
        static_cast<int>(player.position().z) / (Chunk::SIZE / 2)
    );

    if (p.x != old_hcp.x || p.y != old_hcp.y || force) {
        Logger::inst().log_debug("Re-enabling chunks.");

        old_hcp.x = p.x;
        old_hcp.y = p.y;

        for (int i = 0; i < 16; i++)
            visible[i] = -1;

        if (p.x % 2 == 0) p.x--;
        if (p.y % 2 == 0) p.y--;

        p.x /= 2;
        p.y /= 2;

        Point2i pe(p.x + 2, p.y + 2);

        if (p.x < 0) p.x = 0;
        if (p.y < 0) p.y = 0;

        auto& chunks_count = maze.chunks_count();

        if (pe.x > chunks_count.x) pe.x = chunks_count.x;
        if (pe.y > chunks_count.y) pe.y = chunks_count.y;

        for (int i = p.x; i < pe.x; i++)
            for (int j = p.y; j < pe.y; j++) {
                int chunk_num = i + j * chunks_count.x;

                enable_chunk(chunk_num);
            }
    }

    on_tick(delta_time);
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
