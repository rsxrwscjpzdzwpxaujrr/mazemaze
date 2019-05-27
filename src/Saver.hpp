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

class Saver {
public:
    explicit Saver();
    ~Saver();

    void save(Game* game);
    Game* load(gui::MainMenu* mainMenu);

private:
    std::string filename;

    void writeChunk(std::ofstream* stream, Chunk* chunk);
    void readChunk(std::istream* stream, Chunk* chunk);
};

}
