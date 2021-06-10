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

#include "../ITickable.hpp"

#include <string>

#include <SFML/Window/Event.hpp>

#include <SFGUI/Widgets.hpp>

namespace mazemaze {
namespace gui {

class State : public ITickable<void*> {
public:
    const std::string name;

    explicit State(sfg::Desktop& desktop, const std::string& name);
    virtual ~State();

    virtual void center();
    virtual void show(bool show);
    virtual void tick(void*, float delta_time) override;
    virtual void reset_text() = 0;
    virtual sfg::Container::Ptr main_container();

protected:
    sfg::Desktop& desktop;
    sfg::Box::Ptr box;

    virtual void center(sfg::Widget::Ptr widget);
};

}
}
