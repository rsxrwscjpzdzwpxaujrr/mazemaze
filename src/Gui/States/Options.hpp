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

#include <string>

#include <SFML/System/String.hpp>

#include "../State.hpp"

namespace mazemaze {

class Settings;

namespace gui {

class MainMenu;

namespace states {

class Options : public State {
public:
    explicit Options(MainMenu& main_menu, Settings& settings, const std::string& name);
    ~Options() override;

    void reset_text() override final;
    virtual void on_reset_text();

protected:
    Settings& settings;

    sfg::Box::Ptr window_box;

    class Option {
    public:
        explicit Option(const sf::String& label, sfg::Widget::Ptr control);
        explicit Option(sfg::Widget::Ptr widget);

        void change_text(const sf::String& text);
        sfg::Widget::Ptr get_control() const;
        sfg::Widget::Ptr to_widget() const;

    private:
        sfg::Label::Ptr label;
        sfg::Widget::Ptr control;
        sfg::Widget::Ptr widget;
    };

private:
    sfg::Button::Ptr back_button;

    void init_signals(MainMenu& main_menu);
};

}
}
}
