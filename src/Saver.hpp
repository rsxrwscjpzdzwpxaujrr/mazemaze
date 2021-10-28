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

#pragma once

#include <string>
#include <fstream>
#include <mutex>

namespace mazemaze {
namespace gui {

class MainMenu;

}

class Game;
class Chunk;
class Settings;

class Saver {
public:
    static const char version[];

    static bool save_exists(Settings& settings);

    Saver(Settings& settings);
    ~Saver();

    Game* load(gui::MainMenu& main_menu);
    void save();
    void delete_save();

    float last_save_time() const;

    void set_game(Game& game);

private:
    Game* game;
    Settings& settings;
    float m_last_save_time;
    bool virgin;

    std::mutex mutex;

    void save_game  (std::ostream& stream);
    void save_player(std::ostream& stream);
    void save_maze  (std::ostream& stream);
    void save_chunks(std::ostream& stream);

    static std::string get_filename(const Settings& settings);

    static void save_chunk(std::ostream& stream, Chunk& chunk);
    static void load_chunk(std::istream& stream, Chunk& chunk);
};

}
