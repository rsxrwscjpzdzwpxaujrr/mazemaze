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

#include "Gui.hpp"

#include "../Logger.hpp"

#include "State.hpp"

namespace mazemaze {
namespace gui {

Gui::Gui() : background(nullptr),
             state(-1),
             wants_exit(false) {}

Gui::~Gui() {
    remove_states();
}

void
Gui::handle_event(const sf::Event& event) {
    if (event.type == sf::Event::Resized) {
        for (auto state : states)
            state->center();
    }

    on_event(event);

    desktop.HandleEvent(event);
}

void
Gui::on_event(const sf::Event&) {

}

void
Gui::tick(void* _, float delta_time) {
    desktop.Update(delta_time);

    tickable_handler.tick(_, delta_time);
}

void
Gui::render() {
    if (background != nullptr)
        background->render();
}

void
Gui::back() {
    state_stack.pop();
    set_state(state_stack.top(), true);
}

void
Gui::back_to(int dest_state) {
    while (state_stack.top() != dest_state) {
        state_stack.pop();

        if (state_stack.empty()) {
            wants_exit = true;
            return;
        }
    }

    set_state(state_stack.top(), true);
}

int
Gui::add_state(State* state) {
    Logger::inst().log_debug("Adding " + state->name + " state to GUI.");

    states.emplace_back(state);
    states.at(states.size() - 1)->show(false);

    return states.size() - 1;
}

void
Gui::remove_state(int state) {
    if (Gui::state == state)
        set_state(-1);

    delete states.at(state);
    states.erase(states.begin() + state);
}

void
Gui::remove_states() {
    for (auto state : states)
        delete state;

    states.clear();
}

void
Gui::reset_text() {
    Logger::inst().log_debug("Resetting text.");

    for (auto state : states)
        state->reset_text();
}

void
Gui::add_overlay(int state_id) {
    Logger::inst().log_debug("Adding overlay " + get_state(state_id).name);

    states.at(state_id)->show(true);

    tickable_handler.addTickable(states[state_id]);
    overlays.emplace_back(state_id);
}

void
Gui::remove_overlay(int state_id) {
    Logger::inst().log_debug("Removing overlay " + get_state(state_id).name);

    states.at(state_id)->show(false);

    for (auto i = overlays.begin(); i < overlays.end(); i++) {
        if (*i == state_id) {
            tickable_handler.removeTickable(states[state_id]);
            overlays.erase(i);
            break;
        }
    }
}

void
Gui::set_state(int state, bool back) {
    Logger::inst().log_debug("Setting state to " + (state >= 0 ? get_state(state).name : "-1"));

    if (Gui::state >= 0) {
        states[Gui::state]->show(false);
        tickable_handler.removeTickable(states[Gui::state]);
    }

    if (state < 0 && !back)
        state_stack.emplace(state);

    for (int i = 0; i < states.size(); i++) {
        if (i == state) {
            tickable_handler.addTickable(states[i]);
            states[i]->show(true);

            if (!back)
                state_stack.emplace(i);
        }
    }

    Gui::state = state;
}

void
Gui::set_background(Background* background) {
    if (Gui::background)
        tickable_handler.removeTickable(Gui::background);

    if (Gui::background != background)
        tickable_handler.addTickable(background);

    Gui::background = background;
}

void
Gui::exit() {
    wants_exit = true;
}

sfg::Desktop&
Gui::get_desktop() {
    return desktop;
}

int
Gui::get_state() const {
    return state;
}

State&
Gui::get_state(int state) {
    return *states.at(state);
}

bool
Gui::is_wants_exit() const {
    return wants_exit;
}

}
}
