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
        last_save_time(0.0f),
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
    stream.read(reinterpret_cast<char*>(&time), sizeof (float));

    stream.seekg(PLAYER_OFFSET);
    stream.read(reinterpret_cast<char*>(&player_params), sizeof (float) * 6);

    stream.seekg(MAZE_OFFSET);
    stream.read(reinterpret_cast<char*>(maze_params), sizeof (int32_t) * 7);

    game = new Game(main_menu, settings, *this, (maze_params[0] / 2), (maze_params[1] / 2));

    last_save_time = time;
    game->set_time(time);

    Maze& maze = game->get_maze();

    stream.seekg(CHUNKS_OFFSET);

    maze.init_chunks();
    for (int i = 0; i < maze.get_chunks_count(); i++)
        load_chunk(stream, game->get_maze().get_chunks()[i]);

    stream.close();

    Player& player = game->get_player();
    Camera& camera = player.get_camera();

    player.set_x(player_params[0]);
    player.set_y(player_params[1]);
    player.set_z(player_params[2]);
    camera.set_pitch(player_params[3]);
    camera.set_yaw  (player_params[4]);
    camera.set_roll (player_params[5]);

    maze.set_seed(maze_params[2]);

    maze.set_exit_x (maze_params[3]);
    maze.set_exit_y (maze_params[4]);
    maze.set_start_x(maze_params[5]);
    maze.set_start_y(maze_params[6]);

    Logger::inst().log_status(fmt("Save is loaded."));

    game->on_load();

    virgin = false;

    return game;
}

void
Saver::save() {
    if (game == nullptr)
        throw std::logic_error("game is nullptr");

    if (last_save_time == game->get_time()) {
        Logger::inst().log_debug(
            fmt("Trying to save, but game time is equal to last save time. "
                "Last save time is %f.",
                last_save_time));

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
                fmt("Saving completed. Last save time is %f.", last_save_time)
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
Saver::get_last_save_time() const {
    return last_save_time;
}

void
Saver::set_game(Game& game) {
    Saver::game = &game;
}

void
Saver::save_game(std::ostream& stream) {
    float time = game->get_time();

    last_save_time = time;

    stream.seekp(GAME_OFFSET);
    stream.write(reinterpret_cast<char*>(&time), sizeof (float));
}

void
Saver::save_player(std::ostream& stream) {
    Player& player = game->get_player();
    Camera& camera = player.get_camera();

    float player_params[] {
        player.get_x(),
        player.get_y(),
        player.get_z(),
        camera.get_pitch(),
        camera.get_yaw(),
        camera.get_roll()
    };

    stream.seekp(PLAYER_OFFSET);
    stream.write(reinterpret_cast<char*>(&player_params), sizeof (float) * 6);
}

void
Saver::save_maze(std::ostream& stream) {
    Maze& maze = game->get_maze();

    int32_t maze_params[] {
        maze.get_width(),
        maze.get_height(),
        static_cast<int32_t>(maze.get_seed()),
        maze.get_exit_x(),
        maze.get_exit_y(),
        maze.get_start_x(),
        maze.get_start_y()};

    stream.seekp(MAZE_OFFSET);
    stream.write(reinterpret_cast<char*>(&maze_params), sizeof (int32_t) * 7);
}

void
Saver::save_chunks(std::ostream& stream) {
    Maze& maze = game->get_maze();
    Chunk* chunks = maze.get_chunks();

    stream.seekp(CHUNKS_OFFSET);
    for (int i = 0; i < maze.get_chunks_count(); i++)
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
Saver::get_filename(Settings& settings) {
    return settings.get_data_dir() + PATH_SEPARATOR "sav";
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
