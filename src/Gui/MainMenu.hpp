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

#include "Gui.hpp"

namespace mazemaze {

class Game;
class StarSky;
class Settings;
class Saver;

namespace gui {

class State;

namespace states {

class OptionsMenu;

}

class MainMenu : public Gui {
public:
    explicit MainMenu(Settings& settings);
    ~MainMenu() override;

    void on_event(const sf::Event& event) override;

    Game& new_game(int maze_width, int maze_height);
    void start_game();
    void resume_game();
    void stop_game();
    bool is_game_open();
    void show_fps(bool show);
    void show_debug(bool show);

    int  get_options_state() const;
    void set_options_state(states::OptionsMenu& options, int state);
    bool get_show_fps() const;
    bool get_show_debug() const;

private:
    Game* game;
    Saver* saver;
    Background* star_sky_background;
    Settings& settings;

    int main_state;
    int fps_state;
    int debug_state;
    int options_state;

    bool fps_show;
    bool debug_show;

    void setup_game();
};

}
}
