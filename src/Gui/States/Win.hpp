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

namespace gui {

class MainMenu;

namespace states {

class Win : public State {
public:
    explicit Win(MainMenu& main_menu, Game& game);
    ~Win() override;

    void show(bool show) override;
    void reset_text() override;

private:
    sfg::Button::Ptr exit_button;
    sfg::Label::Ptr  win_label;
    sfg::Label::Ptr  win_note_time_label;
    sfg::Label::Ptr  win_note_size_label;
    Game& game;

    void update_labels(Game& game);
};

}
}
}
