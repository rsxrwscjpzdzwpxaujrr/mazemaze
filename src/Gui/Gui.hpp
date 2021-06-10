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

#include <stack>
#include <vector>

#include <SFGUI/Desktop.hpp>

#include "../IRenderable.hpp"
#include "../ITickable.hpp"
#include "../TickableHandler.hpp"

#include "Background.hpp"

namespace mazemaze {
namespace gui {

class State;

class Gui : public ITickable<void*>, public IRenderable {
public:
    Gui();
    ~Gui() override;

    void handle_event(const sf::Event& event);
    virtual void on_event(const sf::Event& event);

    void tick(void*, float delta_time) override;
    void render() override;

    void back();
    void back_to(int dest_state);

    int  add_state(State* state);
    void remove_state(int state);
    void remove_states();

    void reset_text();

    void add_overlay(int state_id);
    void remove_overlay(int state_id);

    void set_state(int state, bool back = false);
    void set_background(Background* background);

    void exit();

    sfg::Desktop& desktop();
    int           state() const;
    State&        state(int state);
    Background&   background() const;
    bool          wants_exit() const;

private:
    sfg::Desktop m_desktop;

    Background* m_background;

    std::vector<State*> states;
    std::stack <int>    state_stack;
    std::vector<int>    overlays;

    TickableHandler<void*> tickable_handler;

    int  m_state;
    bool m_wants_exit;
};

}
}
