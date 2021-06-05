/*
 * Copyright (c) 2021, Мира Странная <rsxrwscjpzdzwpxaujrr@yahoo.com>
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

#include <chrono>

#include "../../Logger.hpp"

namespace mazemaze {
namespace gui {

class MainMenu;

namespace states {

class Debug : public State {
public:
    explicit Debug(MainMenu& main_menu);
    ~Debug() override;

    void tick(float deltaTime) override;
    void show(bool show) override;
    sfg::Container::Ptr getMainContainer() override;
    void resetText() override;

private:
    sfg::Box::Ptr log_box;
    sfg::Window::Ptr window;
    sfg::ScrolledWindow::Ptr scrolled_window;
    std::chrono::system_clock::time_point last_message;

    bool showing;
    bool odd;

    sfg::Widget::Ptr create_log_element(Logger::Message message, bool odd);
};

}
}
}
