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

#include "Options.hpp"

#include <string>

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
    static const int buttonsCount;

    explicit OptionsControls(MainMenu& mainMenu, Settings& settings);
    ~OptionsControls() override;

    void show(bool show) override;
    void tick(float deltatime) override;
    void center();

private:
    class KeyChangeWindow {
    public:
        explicit KeyChangeWindow(OptionsControls& optCtrls);
        ~KeyChangeWindow();

        void tick();

        bool isOpened();
        sfg::Window::Ptr getWindow();

        void center();
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

    sfg::Adjustment::Ptr sensitivityAdjustement;
    sfg::Button::Ptr*    keyButtons;
    sf::String*          keyLabels;

    KeyChangeWindow keyChangeWindow;

    sf::Keyboard::Key  selectedKey;
    std::string*       keyControls;

    void updateKeyButtonLabel(int button);

    void initSignals();

    sf::String getKeyName(sf::Keyboard::Key key);
};

}
}
}
