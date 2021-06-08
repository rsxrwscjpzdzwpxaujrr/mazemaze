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
    static const int buttons_count = 4;

    explicit OptionsControls(MainMenu& main_menu, Settings& settings);
    ~OptionsControls() override;

    void show(bool show) override;
    void tick(void*, float delta_time) override;
    void center() override;
    void on_reset_text() override;

private:
    class KeyChangeWindow {
    public:
        explicit KeyChangeWindow(OptionsControls& opt_ctrls);
        ~KeyChangeWindow();

        void tick();

        bool is_opened();
        sfg::Window::Ptr get_window();

        void reset_text();
        void open(int button);
        void close();

    private:
        OptionsControls& opt_ctrls;

        sfg::Window::Ptr    window;
        sfg::Label::Ptr     label;
        sfg::Button::Ptr    cancel_button;
        sfg::Button::Ptr    ok_button;
        sfg::Separator::Ptr button_separator;

        int button;
        bool opened;

        void init_signals();
    };

    sfg::Scale::Ptr sensitivity_slider;
    sfg::Adjustment::Ptr sensitivity_adjustement;
    std::array<sfg::Button::Ptr, buttons_count> key_buttons;
    std::array<sf::String, buttons_count>       key_labels;

    KeyChangeWindow key_change_window;

    Option sensitivity_opt;
    std::array<Option, buttons_count> key_opts;

    sf::Keyboard::Key  selected_key;
    std::array<std::string, buttons_count> key_controls;

    void update_key_button_label(int button);

    void init_signals();

    sf::String get_key_name(sf::Keyboard::Key key);
};

}
}
}
