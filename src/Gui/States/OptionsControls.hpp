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

#include <string>
#include <array>

#include <SFML/Window.hpp>
#include <SFML/System.hpp>

#include <SFGUI/Widgets.hpp>
#include <SFGUI/Adjustment.hpp>

namespace mazemaze {

class Settings;

namespace gui {

class MainMenu;

namespace states {

class OptionsControls : public Options {
public:
    static const int buttonsCount = 4;

    explicit OptionsControls(MainMenu& mainMenu, Settings& settings);
    ~OptionsControls() override;

    void show(bool show) override;
    void tick(void*, float deltatime) override;
    void center() override;
    void onResetText() override;

private:
    class KeyChangeWindow {
    public:
        explicit KeyChangeWindow(OptionsControls& optCtrls);
        ~KeyChangeWindow();

        void tick();

        bool isOpened();
        sfg::Window::Ptr getWindow();

        void resetText();
        void open(int button);
        void close();

    private:
        OptionsControls& optCtrls;

        sfg::Window::Ptr    window;
        sfg::Label::Ptr     label;
        sfg::Button::Ptr    cancelButton;
        sfg::Button::Ptr    okButton;
        sfg::Separator::Ptr buttonSeparator;

        int button;
        bool opened;

        void initSignals();
    };

    sfg::Scale::Ptr sensitivitySlider;
    sfg::Adjustment::Ptr sensitivityAdjustement;
    std::array<sfg::Button::Ptr, buttonsCount> keyButtons;
    std::array<sf::String, buttonsCount>       keyLabels;

    KeyChangeWindow keyChangeWindow;

    Option sensitivityOpt;
    std::array<Option, buttonsCount> keyOpts;

    sf::Keyboard::Key  selectedKey;
    std::array<std::string, buttonsCount> keyControls;

    void updateKeyButtonLabel(int button);

    void initSignals();

    sf::String getKeyName(sf::Keyboard::Key key);
};

}
}
}
