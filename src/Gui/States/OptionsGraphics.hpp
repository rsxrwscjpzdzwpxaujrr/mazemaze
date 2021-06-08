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

#include "Options.hpp"

#include <SFGUI/SFGUI.hpp>

namespace mazemaze {

class Settings;

namespace gui {

class MainMenu;

namespace states {

class OptionsGraphics : public Options {
public:
    explicit OptionsGraphics(MainMenu& main_menu, Settings& settings);
    ~OptionsGraphics() override;

    void on_reset_text() override;

private:
    sfg::CheckButton::Ptr fullscreen_check;
    sfg::CheckButton::Ptr vsync_check;
    sfg::ComboBox::Ptr    antialiasing_combo;
    sfg::ComboBox::Ptr    style_combo;
    sfg::CheckButton::Ptr camera_bobbing_check;

    Option fullscreen_opt;
    Option vsync_opt;
    Option antialiasing_opt;
    Option style_opt;
    Option camera_bobbing_opt;

    void init_signals();
    void init_antialiasing_combo();
    void init_options();
};

}
}
}
