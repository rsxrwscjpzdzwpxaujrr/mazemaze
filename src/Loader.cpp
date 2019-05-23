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

#include "Loader.hpp"

#include <fstream>

#include "Game.hpp"
#include "Chunk.hpp"

namespace mazemaze {

Loader::Loader(std::string filename) : filename(filename) {}

Loader::~Loader() = default;

Game*
Loader::load(MainMenu* mainMenu) {
    std::ifstream stream;

    stream.open(filename, std::ios::in | std::ios::binary);

    if (stream.is_open()) {
        int32_t mazeParams[5];

        stream.read(reinterpret_cast<char*>(mazeParams), sizeof (int32_t) * 5);
        stream.seekg(0x400);

        Game* game = new Game(mainMenu, (mazeParams[0] / 2), (mazeParams[1] / 2));
        Maze* maze = game->getMaze();

        maze->setSeed(mazeParams[2]);

        for (int i = 0; i < maze->getChunksCount(); i++)
            readChunk(&stream, &game->getMaze()->getChunks()[i]);

        std::ios::seekdir dir = stream.cur;

        float time;
        stream.read(reinterpret_cast<char*>(&time), sizeof (float));
        game->setTime(time);
        stream.seekg(0x400, dir);

        Player* player = game->getPlayer();
        float playerParams[3];

        stream.read(reinterpret_cast<char*>(&playerParams), sizeof (float) * 3);
        player->setX(playerParams[0]);
        player->setY(playerParams[1]);
        player->setZ(playerParams[2]);
        stream.seekg(0x800, dir);

        stream.close();

        game->onLoad();

        return game;
    }

    return nullptr;
}

void
Loader::readChunk(std::istream* stream, Chunk* chunk) {
    const int byteCount = (Chunk::SIZE * Chunk::SIZE) / 8;
    char bytes[byteCount];

    stream->read(bytes, byteCount);

    for (unsigned int i = 0; i < Chunk::SIZE; i++)
        for (unsigned int j = 0; j < Chunk::SIZE; j++) {
            unsigned int block = i * Chunk::SIZE + j;

            chunk->setOpened(i, j, (bytes[block / 8] & (1 << block % 8)) != 0);
        }
}

}
