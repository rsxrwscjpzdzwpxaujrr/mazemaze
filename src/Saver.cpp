/*
 * Copyright (c) 2019-2020, Мира Странная <miraityan2004@gmail.com>
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

#include "Game.hpp"
#include "Chunk.hpp"
#include "Settings.hpp"
#include "path_separator.hpp"

namespace mazemaze {

Saver::Saver() : version{1, 0, 0} {}

Saver::~Saver() = default;

void
Saver::save(Game& game, Settings& settings) {
    std::ofstream stream;

    stream.open(getFilename(settings), std::ios::out | std::ios::trunc | std::ios::binary);

    if (stream.is_open()) {
        Maze&   maze   = game.getMaze();
        Player& player = game.getPlayer();
        Camera& camera = player.getCamera();
        float   time   = game.getTime();

        float playerParams[] {player.getX(),
                              player.getY(),
                              player.getZ(),
                              camera.getPitch(),
                              camera.getYaw(),
                              camera.getRoll()};

        stream.write(version, sizeof (char) * 3);
        stream.seekp(0x100);

        stream.write(reinterpret_cast<char*>(&time), sizeof (float));
        stream.seekp(0x200);

        stream.write(reinterpret_cast<char*>(&playerParams), sizeof (float) * 6);
        stream.seekp(0x300);

        if (!game.isChunksSaved()) {
            Chunk*  chunks = maze.getChunks();
            int32_t mazeParams[] {maze.getWidth(),
                                 maze.getHeight(),
                                 static_cast<int32_t>(maze.getSeed()),
                                 maze.getExitX(),
                                 maze.getExitY(),
                                 maze.getStartX(),
                                 maze.getStartY()};

            stream.write(reinterpret_cast<char*>(&mazeParams), sizeof (int32_t) * 7);

            for (int i = 0; i < maze.getChunksCount(); i++)
                writeChunk(stream, chunks[i]);

            game.setChunksSaved();
        }

        stream.close();
    }
}

void
Saver::deleteSave(Settings& settings) {
    if (saveExists(settings))
        std::remove(getFilename(settings).c_str());
}

bool
Saver::saveExists(Settings& settings) {
    std::FILE* file = fopen(getFilename(settings).c_str(), "r");
    bool exist = file != nullptr;

    if (exist)
        fclose(file);

    return exist;
}

Game*
Saver::load(gui::MainMenu& mainMenu, Settings& settings) {
    std::ifstream stream;

    stream.open(getFilename(settings), std::ios::in | std::ios::binary);

    if (stream.is_open()) {
        char version[3];
        float time;
        float playerParams[6];
        int32_t mazeParams[7];

        stream.read(version, sizeof (char) * 3);
        stream.seekg(0x100);

        if (version[0] != Saver::version[0])
            return nullptr;

        stream.read(reinterpret_cast<char*>(&time), sizeof (float));
        stream.seekg(0x200);

        stream.read(reinterpret_cast<char*>(&playerParams), sizeof (float) * 6);
        stream.seekg(0x300);

        stream.read(reinterpret_cast<char*>(mazeParams), sizeof (int32_t) * 7);

        Game* game = new Game(mainMenu, settings, (mazeParams[0] / 2), (mazeParams[1] / 2));
        Maze& maze = game->getMaze();

        for (int i = 0; i < maze.getChunksCount(); i++)
            readChunk(stream, game->getMaze().getChunks()[i]);

        stream.close();

        Player& player = game->getPlayer();
        Camera& camera = player.getCamera();

        game->setTime(time);

        player.setX(playerParams[0]);
        player.setY(playerParams[1]);
        player.setZ(playerParams[2]);
        camera.setPitch(playerParams[3]);
        camera.setYaw  (playerParams[4]);
        camera.setRoll (playerParams[5]);

        maze.setSeed(mazeParams[2]);

        maze.setExitX (mazeParams[3]);
        maze.setExitY (mazeParams[4]);
        maze.setStartX(mazeParams[5]);
        maze.setStartY(mazeParams[6]);

        game->onLoad();
        game->setChunksSaved();

        return game;
    }

    return nullptr;
}

std::string
Saver::getFilename(Settings& settings) {
    return settings.getDataDir() + PATH_SEPARATOR "sav";
}

void
Saver::writeChunk(std::ostream& stream, Chunk& chunk) {
    const int byteCount = (Chunk::SIZE * Chunk::SIZE) / 8;
    char bytes[byteCount] {0};

    for (unsigned int i = 0; i < Chunk::SIZE; i++)
        for (unsigned int j = 0; j < Chunk::SIZE; j++) {
            unsigned int block = i * Chunk::SIZE + j;

            if (chunk.getOpened(i, j))
                bytes[block / 8] |= 1 << block % 8;
        }

    stream.write(bytes, byteCount);
}

void
Saver::readChunk(std::istream& stream, Chunk& chunk) {
    const int byteCount = (Chunk::SIZE * Chunk::SIZE) / 8;
    char bytes[byteCount];

    stream.read(bytes, byteCount);

    for (unsigned int i = 0; i < Chunk::SIZE; i++)
        for (unsigned int j = 0; j < Chunk::SIZE; j++) {
            unsigned int block = i * Chunk::SIZE + j;

            chunk.setOpened(i, j, (bytes[block / 8] & (1 << block % 8)) != 0);
        }
}

}
