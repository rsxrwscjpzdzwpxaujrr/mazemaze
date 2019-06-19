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

#include "Saver.hpp"

#include <cstdio>

#include "Game.hpp"
#include "Chunk.hpp"

namespace mazemaze {

Saver::Saver() : filename("sav") {}

Saver::~Saver() = default;

void
Saver::save(Game* game) {
    std::ofstream stream;

    stream.open(filename, std::ios::out | std::ios::trunc | std::ios::binary);

    if (stream.is_open()) {
        Maze*   maze   = game->getMaze();
        Player* player = game->getPlayer();
        Camera* camera = player->getCamera();
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
                              player->getZ(),
                              camera->getPitch(),
                              camera->getYaw(),
                              camera->getRoll()};

        stream.write(reinterpret_cast<char*>(&playerParams), sizeof (float) * 6);
        stream.seekp(0x800, dir);
    }

    stream.close();
}

void
Saver::deleteSave(Game* game) {
    if (fopen(filename.c_str(), "r"))
        std::remove(filename.c_str());
}

Game*
Saver::load(gui::MainMenu* mainMenu, Settings* settings) {
    std::ifstream stream;

    stream.open(filename, std::ios::in | std::ios::binary);

    if (stream.is_open()) {
        int32_t mazeParams[5];

        stream.read(reinterpret_cast<char*>(mazeParams), sizeof (int32_t) * 5);
        stream.seekg(0x400);

        Game* game = new Game(mainMenu, settings, (mazeParams[0] / 2), (mazeParams[1] / 2));
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
        Camera* camera = player->getCamera();
        float playerParams[6];

        stream.read(reinterpret_cast<char*>(&playerParams), sizeof (float) * 6);
        player->setX(playerParams[0]);
        player->setY(playerParams[1]);
        player->setZ(playerParams[2]);
        camera->setPitch(playerParams[3]);
        camera->setYaw  (playerParams[4]);
        camera->setRoll (playerParams[5]);
        stream.seekg(0x800, dir);

        stream.close();

        game->onLoad();

        return game;
    }

    return nullptr;
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

void
Saver::readChunk(std::istream* stream, Chunk* chunk) {
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
