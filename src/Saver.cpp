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
        m_last_save_time(0.0f),
        virgin(true) {}

Saver::~Saver() = default;

Game*
Saver::load(gui::MainMenu& main_menu) {
    Logger::inst().log_debug(fmt("Loading save."));

    std::lock_guard<std::mutex> lock(mutex);

    std::ifstream stream;

    stream.open(get_filename(settings), std::ios::in | std::ios::binary);
    stream.exceptions(std::ios::failbit | std::ios::badbit | std::ios::eofbit);

    char version[3];
    float time;
    float player_params[6];
    int32_t maze_params[7];

    stream.seekg(VERSION_OFFSET);
    stream.read(version, sizeof (char) * 3);

    if (version[0] != Saver::version[0])
        throw std::runtime_error(fmt("Incompatible save version %d.%d.%d", version[0],
                                                                           version[1],
                                                                           version[2]));

    stream.seekg(GAME_OFFSET);
    stream.read(reinterpret_cast<char*>(&time), sizeof (time));

    stream.seekg(PLAYER_OFFSET);
    stream.read(reinterpret_cast<char*>(&player_params), sizeof (player_params));

    stream.seekg(MAZE_OFFSET);
    stream.read(reinterpret_cast<char*>(maze_params), sizeof (maze_params));

    game = new Game(main_menu, settings, *this, Point2i((maze_params[0] / 2), (maze_params[1] / 2)));

    m_last_save_time = time;
    game->set_time(time);

    Maze& maze = game->maze();

    stream.seekg(CHUNKS_OFFSET);

    maze.init_chunks();

    Point2i chunks_count = maze.chunks_count();

    for (int i = 0; i < chunks_count.x * chunks_count.y; i++)
        load_chunk(stream, game->maze().chunks()[i]);

    stream.close();

    auto& player = game->player();
    auto& player_pos = player.position();
    auto& rotation = player.camera().rotation();

    player_pos.x = player_params[0];
    player_pos.y = player_params[1];
    player_pos.z = player_params[2];
    rotation.set_pitch(player_params[3]);
    rotation.set_yaw  (player_params[4]);
    rotation.set_roll (player_params[5]);
    maze.set_seed(maze_params[2]);

    auto& exit  = maze.exit();
    auto& start = maze.start();

    exit.x = maze_params[3];
    exit.y = maze_params[4];

    start.x = maze_params[5];
    start.y = maze_params[6];

    Logger::inst().log_status(fmt("Save is loaded."));

    game->on_load();

    virgin = false;

    return game;
}

void
Saver::save() {
    if (game == nullptr)
        throw std::logic_error("game is nullptr");

    if (m_last_save_time == game->time()) {
        Logger::inst().log_debug(
            fmt("Trying to save, but game time is equal to last save time. "
                "Last save time is %f.",
                m_last_save_time));

        return;
    }

    if (!mutex.try_lock()) {
        Logger::inst().log_debug(fmt("Trying to save, but mutex is locked."));

        return;
    }

    std::thread([this] {
        Logger::inst().log_debug(fmt("Saving. Saver is %svirgin.", virgin ? "" : "not "));

        try {
            std::ofstream stream;

            stream.exceptions(std::ios::failbit | std::ios::badbit);

            std::ios::openmode mode = std::ios::in | std::ios::out | std::ios::binary;

            if (virgin)
                mode |= std::ios::trunc;

            stream.open(get_filename(settings), mode);

            stream.seekp(VERSION_OFFSET);
            stream.write(version, sizeof (char) * 3);

            save_game(stream);
            save_player(stream);

            if (virgin) {
                save_maze(stream);
                save_chunks(stream);
            }

            stream.close();

            virgin = false;

            Logger::inst().log_status(
                fmt("Saving completed. Last save time is %f.", m_last_save_time)
            );
        } catch (const std::ofstream::failure& e) {
            Logger::inst().log_error(
                fmt("Saving error: %s, code: %d.", e.what(), e.code())
            );
        }

        mutex.unlock();
    }).detach();
}

void
Saver::delete_save() {
    if (save_exists(settings))
        std::remove(get_filename(settings).c_str());
}

float
Saver::last_save_time() const {
    return m_last_save_time;
}

void
Saver::set_game(Game& game) {
    Saver::game = &game;
}

void
Saver::save_game(std::ostream& stream) {
    float time = game->time();

    m_last_save_time = time;

    stream.seekp(GAME_OFFSET);
    stream.write(reinterpret_cast<char*>(&time), sizeof (float));
}

void
Saver::save_player(std::ostream& stream) {
    auto& player = game->player();
    auto& player_pos = player.position();
    auto& rotation = player.camera().rotation();

    float player_params[] {
        player_pos.x,
        player_pos.y,
        player_pos.z,
        rotation.pitch(),
        rotation.yaw(),
        rotation.roll()
    };

    stream.seekp(PLAYER_OFFSET);
    stream.write(reinterpret_cast<char*>(&player_params), sizeof (player_params));
}

void
Saver::save_maze(std::ostream& stream) {
    auto& maze  = game->maze();
    auto  size  = maze.size();
    auto  exit  = maze.exit();
    auto  start = maze.start();

    int32_t maze_params[] {
        size .x, size.y,
        static_cast<int32_t>(maze.seed()),
        exit .x, exit.y,
        start.x, start.y
    };

    stream.seekp(MAZE_OFFSET);
    stream.write(reinterpret_cast<char*>(&maze_params), sizeof (int32_t) * 7);
}

void
Saver::save_chunks(std::ostream& stream) {
    auto& maze = game->maze();
    auto* chunks = maze.chunks();
    auto  chunks_count = maze.chunks_count();

    stream.seekp(CHUNKS_OFFSET);
    for (int i = 0; i < chunks_count.x * chunks_count.y; i++)
        save_chunk(stream, chunks[i]);
}

bool
Saver::save_exists(Settings& settings) {
    std::FILE* file = fopen(get_filename(settings).c_str(), "r");
    bool exist = file != nullptr;

    if (exist)
        fclose(file);

    return exist;
}

std::string
Saver::get_filename(const Settings& settings) {
    return settings.data_dir() + PATH_SEPARATOR "sav";
}

void
Saver::save_chunk(std::ostream& stream, Chunk& chunk) {
    const int byte_count = (Chunk::SIZE * Chunk::SIZE) / 8;
    char bytes[byte_count] {0};

    for (unsigned int i = 0; i < Chunk::SIZE; i++)
        for (unsigned int j = 0; j < Chunk::SIZE; j++) {
            unsigned int block = i * Chunk::SIZE + j;

            if (chunk.get_opened(i, j))
                bytes[block / 8] |= 1 << block % 8;
        }

    stream.write(bytes, byte_count);
}

void
Saver::load_chunk(std::istream& stream, Chunk& chunk) {
    const int byte_count = (Chunk::SIZE * Chunk::SIZE) / 8;
    char bytes[byte_count];

    stream.read(bytes, byte_count);

    for (unsigned int i = 0; i < Chunk::SIZE; i++)
        for (unsigned int j = 0; j < Chunk::SIZE; j++) {
            unsigned int block = i * Chunk::SIZE + j;

            chunk.set_opened(i, j, (bytes[block / 8] & (1 << block % 8)) != 0);
        }
}

}
