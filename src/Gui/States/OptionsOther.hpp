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

class OptionsOther : public Options {
public:
    explicit OptionsOther(MainMenu& main_menu, Settings& settings);
    ~OptionsOther() override;

    void on_reset_text() override;

private:
    sfg::ComboBox::Ptr    lang_combo;
    sfg::CheckButton::Ptr autosave_check;
    sfg::CheckButton::Ptr show_fps_check;

    Option lang_opt;
    Option autosave_opt;
    Option show_fps_opt;

    void init_signals();
    void init_options();
};

}
}
}
