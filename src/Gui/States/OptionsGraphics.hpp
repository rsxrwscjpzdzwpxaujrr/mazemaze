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
    explicit OptionsGraphics(MainMenu& mainMenu, Settings& settings);
    ~OptionsGraphics() override;

    //void resetText() override;
    void onResetText() override;

private:
    sfg::CheckButton::Ptr fullscreenCheck;
    sfg::CheckButton::Ptr vsyncCheck;
    sfg::ComboBox::Ptr    antialiasingCombo;
    sfg::ComboBox::Ptr    styleCombo;
    sfg::CheckButton::Ptr cameraBobbingCheck;

    Option fullscreenOpt;
    Option vsyncOpt;
    Option antialiasingOpt;
    Option styleOpt;
    Option cameraBobbingOpt;

    void initSignals();
    void initAntialiasingCombo();
    void initOptions();
};

}
}
}
