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

#include "../State.hpp"

#include <SFGUI/SFGUI.hpp>

namespace mazemaze {

class Settings;

namespace gui {

class MainMenu;

namespace states {

class OptionsGraphics : public State {
public:
    explicit OptionsGraphics(MainMenu& mainMenu, Settings& settings);
    ~OptionsGraphics() override;

private:
    Settings& settings;

    sfg::Button::Ptr      backButton;
    sfg::CheckButton::Ptr fullscreenCheck;
    sfg::CheckButton::Ptr vsyncCheck;
    sfg::ComboBox::Ptr    antialiasingCombo;
    sfg::ComboBox::Ptr    styleCombo;

    sfg::Box::Ptr addToOptionsList(const sf::String& label, sfg::Widget::Ptr widget);

    void initSignals(MainMenu& mainMenu);
    void initAntialiasingCombo();
    void initOptions();
};

}
}
}
