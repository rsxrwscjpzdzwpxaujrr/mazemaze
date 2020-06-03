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
    static const char version[];

    static bool saveExists(Settings& settings);

    Saver(Settings& settings);
    ~Saver();

    Game* load(gui::MainMenu& mainMenu);
    void save();
    void deleteSave();

    float getLastSaveTime();

    void setGame(Game& game);

private:
    Game* game;
    Settings& settings;
    float lastSaveTime;

    void saveGame  (std::ostream& stream);
    void savePlayer(std::ostream& stream);
    void saveMaze  (std::ostream& stream);
    void saveChunks(std::ostream& stream);

    static std::string getFilename(Settings& settings);

    static void saveChunk(std::ostream& stream, Chunk& chunk);
    static void loadChunk(std::istream& stream, Chunk& chunk);
};

}
