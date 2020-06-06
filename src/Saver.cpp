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

#include <thread>

#include "Game.hpp"
#include "Chunk.hpp"
#include "Settings.hpp"
#include "path_separator.hpp"
#include "utils.hpp"
#include "Logger.hpp"

#define VERSION_OFFSET 0x0
#define GAME_OFFSET    0x100
#define PLAYER_OFFSET  0x200
#define MAZE_OFFSET    0x300
#define CHUNKS_OFFSET  0x318

namespace mazemaze {

const char Saver::version[] = {1, 0, 0};

Saver::Saver(Settings& settings) :
        game(nullptr),
        settings(settings),
        lastSaveTime(0.0f),
        virgin(true) {}

Saver::~Saver() = default;

Game*
Saver::load(gui::MainMenu& mainMenu) {
    std::ifstream stream;

    stream.open(getFilename(settings), std::ios::in | std::ios::binary);
    stream.exceptions(std::ios::failbit | std::ios::badbit | std::ios::eofbit);

    char version[3];
    float time;
    float playerParams[6];
    int32_t mazeParams[7];

    stream.seekg(VERSION_OFFSET);
    stream.read(version, sizeof (char) * 3);

    if (version[0] != Saver::version[0])
        throw std::runtime_error(format("Incompatible save version %d.%d.%d", version[0],
                                                                              version[1],
                                                                              version[2]));

    stream.seekg(GAME_OFFSET);
    stream.read(reinterpret_cast<char*>(&time), sizeof (float));

    stream.seekg(PLAYER_OFFSET);
    stream.read(reinterpret_cast<char*>(&playerParams), sizeof (float) * 6);

    stream.seekg(MAZE_OFFSET);
    stream.read(reinterpret_cast<char*>(mazeParams), sizeof (int32_t) * 7);

    game = new Game(mainMenu, settings, *this, (mazeParams[0] / 2), (mazeParams[1] / 2));

    lastSaveTime = time;
    game->setTime(time);

    Maze& maze = game->getMaze();

    stream.seekg(CHUNKS_OFFSET);
    for (int i = 0; i < maze.getChunksCount(); i++)
        loadChunk(stream, game->getMaze().getChunks()[i]);

    stream.close();

    Player& player = game->getPlayer();
    Camera& camera = player.getCamera();

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

    virgin = false;

    return game;
}

void
Saver::save() {
    if (game == nullptr)
        throw std::logic_error("game is nullptr");

    if (lastSaveTime == game->getTime()) {
        Logger::inst().log_debug(
            format("Trying to save, but game time is equal to last save time. "
                   "Last save time is %f.",
                   lastSaveTime));

        return;
    }

    std::thread([this] {
        Logger::inst().log_debug(format("Saving. Saver is %svirgin.", virgin ? "" : "not "));

        std::ofstream stream;

        stream.exceptions(std::ios::failbit | std::ios::badbit);

        std::ios::openmode mode = std::ios::in | std::ios::out | std::ios::binary;

        if (virgin)
            mode |= std::ios::trunc;

        stream.open(getFilename(settings), mode);

        stream.seekp(VERSION_OFFSET);
        stream.write(version, sizeof (char) * 3);

        saveGame(stream);
        savePlayer(stream);

        if (virgin) {
            saveMaze(stream);
            saveChunks(stream);
        }

        virgin = false;

        stream.close();

        Logger::inst().log_debug(format("Saving completed. Last save time is %f.", lastSaveTime));
    }).detach();
}

void
Saver::deleteSave() {
    if (saveExists(settings))
        std::remove(getFilename(settings).c_str());
}

float
Saver::getLastSaveTime() const {
    return lastSaveTime;
}

void
Saver::setGame(Game& game) {
    Saver::game = &game;
}

void
Saver::saveGame(std::ostream& stream) {
    float time = game->getTime();

    lastSaveTime = time;

    stream.seekp(GAME_OFFSET);
    stream.write(reinterpret_cast<char*>(&time), sizeof (float));
}

void
Saver::savePlayer(std::ostream& stream) {
    Player& player = game->getPlayer();
    Camera& camera = player.getCamera();

    float playerParams[] {
        player.getX(),
        player.getY(),
        player.getZ(),
        camera.getPitch(),
        camera.getYaw(),
        camera.getRoll()
    };

    stream.seekp(PLAYER_OFFSET);
    stream.write(reinterpret_cast<char*>(&playerParams), sizeof (float) * 6);
}

void
Saver::saveMaze(std::ostream& stream) {
    Maze& maze = game->getMaze();

    int32_t mazeParams[] {
        maze.getWidth(),
        maze.getHeight(),
        static_cast<int32_t>(maze.getSeed()),
        maze.getExitX(),
        maze.getExitY(),
        maze.getStartX(),
        maze.getStartY()};

    stream.seekp(MAZE_OFFSET);
    stream.write(reinterpret_cast<char*>(&mazeParams), sizeof (int32_t) * 7);
}

void
Saver::saveChunks(std::ostream& stream) {
    Maze& maze = game->getMaze();
    Chunk* chunks = maze.getChunks();

    stream.seekp(CHUNKS_OFFSET);
    for (int i = 0; i < maze.getChunksCount(); i++)
        saveChunk(stream, chunks[i]);
}

bool
Saver::saveExists(Settings& settings) {
    std::FILE* file = fopen(getFilename(settings).c_str(), "r");
    bool exist = file != nullptr;

    if (exist)
        fclose(file);

    return exist;
}

std::string
Saver::getFilename(Settings& settings) {
    return settings.getDataDir() + PATH_SEPARATOR "sav";
}

void
Saver::saveChunk(std::ostream& stream, Chunk& chunk) {
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
Saver::loadChunk(std::istream& stream, Chunk& chunk) {
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
