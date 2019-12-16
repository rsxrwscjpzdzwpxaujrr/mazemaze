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

#pragma once

#include <string>
#include <fstream>

namespace mazemaze {
namespace gui {

class MainMenu;

}

class Game;
class Chunk;
class Settings;

class Saver {
public:
    void save(Game& game, Settings& settings);
    void deleteSave(Settings& settings);
    bool saveExists(Settings& settings);
    Game* load(gui::MainMenu& mainMenu, Settings& setiings);

    Saver(Saver const&) = delete;
    void operator= (Saver const&) = delete;

    static Saver& getInstance() {
        static Saver instance;
        return instance;
    }

private:
    const char version[3];

    Saver();
    ~Saver();

    std::string getFilename(Settings& settings);

    void writeChunk(std::ostream& stream, Chunk& chunk);
    void readChunk(std::istream& stream, Chunk& chunk);
};

}
