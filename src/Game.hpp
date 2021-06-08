/*
 * Copyright (c) 2018-2021, Мира Странная <rsxrwscjpzdzwpxaujrr@yahoo.com>
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

#include "Gui/Background.hpp"

#include "Maze.hpp"
#include "Player.hpp"
#include "IRenderable.hpp"
#include "ITickable.hpp"
#include "TickableHandler.hpp"

namespace mazemaze {
namespace gui {

class MainMenu;

}

class Camera;
class Settings;
class MazeRenderer;
class Saver;

class Game : public gui::Background {
public:
    explicit Game(
        gui::MainMenu& main_menu,
        Settings& settings,
        Saver& saver,
        int maze_width,
        int maze_height
    );

    ~Game() override;

    void new_game();
    void on_load();

    void render() override;
    void tick(void*, float delta_time) override;
    void open_gui();
    void stop();

    void set_paused(bool paused);
    void set_won(bool won);
    void set_time(float time);
    void set_renderer(int id);

    bool is_paused() const;
    bool is_won() const;
    bool is_loaded() const;

    float get_time() const;
    Maze& get_maze();
    Player& get_player();
    Camera* get_camera() override;
    MazeRenderer& get_renderer() const;
    Settings& get_settings() const;

private:
    Maze maze;
    int maze_renderer;
    MazeRenderer* maze_renderers[16];
    Player player;
    Settings& settings;
    Saver& saver;
    TickableHandler<Game&> tickable_handler;

    gui::MainMenu& main_menu;

    int pause_state;
    int won_state;

    bool  paused;
    bool  won;
    bool  old_pause_key_state;
    float time;
    bool  loaded;

    unsigned int gen_seed();
};

}
