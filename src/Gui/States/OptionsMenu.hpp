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

class Settings;

namespace gui {

class MainMenu;

namespace states {

class OptionsMenu : public State {
public:
    explicit OptionsMenu(MainMenu& main_menu, Settings& settings);
    ~OptionsMenu() override;

    void reset_text() override;

private:
    sfg::Button::Ptr graphics_button;
    sfg::Button::Ptr controls_button;
    sfg::Button::Ptr other_button;
    sfg::Button::Ptr back_button;

    int graphics_state;
    int controls_state;
    int other_state;

    void init_signals(MainMenu& main_menu);
};

}
}
}
