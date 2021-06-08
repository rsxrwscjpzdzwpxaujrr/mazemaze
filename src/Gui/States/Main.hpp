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

#include "../State.hpp"

namespace mazemaze {

class Game;
class Settings;

namespace gui {

class MainMenu;

namespace states {

class Main : public State {
public:
    explicit Main(MainMenu& main_menu, Settings& settings);
    ~Main() override;

    void show(bool show) override;
    void update_buttons(bool save_exists);

    void center() override;
    void reset_text() override;

private:
    sfg::Button::Ptr resume_button;
    sfg::Button::Ptr new_game_button;
    sfg::Button::Ptr options_button;
    sfg::Button::Ptr about_button;
    sfg::Button::Ptr exit_button;

    Settings& settings;

    int new_game_state;
    int options_state;
    int about_state;

    bool showing;

    void init_signals(MainMenu& main_menu);
};

}
}
}
