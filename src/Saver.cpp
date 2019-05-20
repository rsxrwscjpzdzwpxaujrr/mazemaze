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

#include <fstream>

#include "Game.hpp"
#include "Chunk.hpp"

#include "Saver.hpp"

namespace mazemaze {

Saver::Saver(Game* game, std::string filename) {
    Saver::game = game;
    Saver::filename = filename;
}

void
Saver::save() {
    std::ofstream stream;

    stream.open(filename, std::ios::out | std::ios::trunc | std::ios::binary);

    if (stream.is_open()) {
        Maze*   maze   = game->getMaze();
        Player* player = game->getPlayer();
        Chunk*  chunks = maze->getChunks();

        int32_t mazeParams[] {maze->getWidth(),
                              maze->getHeight(),
                              static_cast<int32_t>(maze->getSeed()),
                              maze->getExitX(),
                              maze->getExitY()};

        stream.write(reinterpret_cast<char*>(&mazeParams), sizeof (int32_t) * 5);
        stream.seekp(0x400, std::ios::beg);

        for (int i = 0; i < maze->getChunksCount(); i++)
            writeChunk(&stream, &chunks[i]);

        std::ios::seekdir dir = stream.cur;

        float time = game->getTime();
        stream.write(reinterpret_cast<char*>(&time), sizeof (float));
        stream.seekp(0x400, dir);

        float playerParams[] {player->getX(),
                              player->getY(),
                              player->getZ()};

        stream.write(reinterpret_cast<char*>(&playerParams), sizeof (float) * 3);
        stream.seekp(0x800, dir);
    }

    stream.close();
}

void
Saver::writeChunk(std::ofstream* stream, Chunk* chunk) {
    const int byteCount = (Chunk::SIZE * Chunk::SIZE) / 8;
    char bytes[byteCount] {0};

    for (unsigned int i = 0; i < Chunk::SIZE; i++)
        for (unsigned int j = 0; j < Chunk::SIZE; j++) {
            unsigned int block = i * Chunk::SIZE + j;

            if (chunk->getOpened(i, j))
                bytes[block / 8] |= 1 << block % 8;
        }

    stream->write(bytes, byteCount);
}

}
