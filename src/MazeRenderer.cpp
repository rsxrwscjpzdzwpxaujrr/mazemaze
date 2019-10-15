/*
 * Copyright (c) 2018-2019, Мира Странная <miraityan2004@gmail.com>
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

namespace mazemaze {

MazeRenderer::MazeRenderer(Game& game) : maze(game.getMaze()),
                                         deleted(true),
                                         oldHcpX(-1),
                                         oldHcpY(-1) {}

MazeRenderer::~MazeRenderer() {
    if (!deleted)
        disable();
}

void
MazeRenderer::enable() {
    visible  = new int[16] {-1};
    compiled = new bool[maze.getChunksCount()] {false};
    drawList = glGenLists(maze.getChunksCount());

    setStates();
    onEnable();

    GraphicEngine::getInstance().setOnSetStatesCallback([this] {
        this->setStates();
    });

    deleted = false;
}

void
MazeRenderer::disable() {
    GraphicEngine::getInstance().setOnSetStatesCallback([] {});

    onDisable();

    glDeleteLists(drawList, maze.getChunksCount());

    delete [] visible;
    delete [] compiled;

    deleted = true;
}

void
MazeRenderer::tick(float deltaTime, float playerX, float playerY, bool force) {
    int pX = static_cast<int>(playerX) / (Chunk::SIZE / 2);
    int pY = static_cast<int>(playerY) / (Chunk::SIZE / 2);

    if (pX != oldHcpX || pY != oldHcpY || force) {
        oldHcpX = pX;
        oldHcpY = pY;

        for (int i = 0; i < 16; i++)
            visible[i] = -1;

        if (pX % 2 == 0) pX--;
        if (pY % 2 == 0) pY--;

        pX /= 2;
        pY /= 2;

        int peX = pX + 2;
        int peY = pY + 2;

        if (pX < 0) pX = 0;
        if (pY < 0) pY = 0;

        if (peX > maze.getChunksX()) peX = maze.getChunksX();
        if (peY > maze.getChunksY()) peY = maze.getChunksY();

        for (int i = pX; i < peX; i++)
            for (int j = pY; j < peY; j++) {
                int chunkNum = i + j * maze.getChunksX();

                enableChunk(chunkNum);
            }
    }

    onTick(deltaTime);
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

    renderChunks(chunks);
}

void
MazeRenderer::setStates() {
}

void
MazeRenderer::onEnable() {
}

void
MazeRenderer::onDisable() {
}

void
MazeRenderer::enableChunk(int num) {
    if (!compiled[num])
        compileChunk(num);

    for (int i = 0; i < 16; i++) {
        if (visible[i] == -1) {
            visible[i] = num;
            break;
        }
    }
}

void
MazeRenderer::renderChunks(int chunks[]) {
    for (; *chunks != -1; chunks++)
        glCallList(drawList + *chunks);
}

}
