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

#include "State.hpp"

#include "../GraphicEngine.hpp"

namespace mazemaze {
namespace gui {

State::State(sfg::Desktop& desktop, const std::string& name) :
        name(name),
        desktop(desktop),
        box(sfg::Box::Create()) {}

State::~State() {
    desktop.Remove(getMainContainer());
}

void
State::center() {
    center(getMainContainer());
}

void
State::center(sfg::Widget::Ptr widget) {
    sf::Vector2f widgetSize(widget->GetAllocation().width, widget->GetAllocation().height);
    sf::Vector2f windowSize =
            static_cast<sf::Vector2f>(GraphicEngine::inst().getWindow().getSize());

    widget->SetPosition((windowSize - widgetSize) / 2.0f);
}

void
State::show(bool show) {
    getMainContainer()->Show(show);
}

void
State::tick(void*, float) {

}

sfg::Container::Ptr
State::getMainContainer() {
    return box;
}

}
}
