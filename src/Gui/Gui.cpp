/*
 * Copyright (c) 2019-2020, Мира Странная <rsxrwscjpzdzwpxaujrr@yahoo.com>
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
             wantExit(false) {}

Gui::~Gui() {
    removeStates();
}

void
Gui::handleEvent(const sf::Event& event) {
    if (event.type == sf::Event::Resized) {
        for (unsigned int i = 0; i < states.size(); i++) {
            states[i]->center();
        }
    }

    onEvent(event);

    desktop.HandleEvent(event);
}

void
Gui::onEvent(const sf::Event&) {

}

void
Gui::tick(float deltaTime) {
    desktop.Update(deltaTime);

    if (state > 0)
        states[state]->tick(deltaTime);

    if (background != nullptr)
        background->tick(deltaTime);

    for (auto i = overlays.begin(); i < overlays.end(); i++) {
        states[*i]->tick(deltaTime);
    }
}

void
Gui::render() {
    if (background != nullptr)
        background->render();
}

void
Gui::back() {
    stateStack.pop();
    setState(stateStack.top(), true);
}

void
Gui::backTo(int destState) {
    while (stateStack.top() != destState) {
        stateStack.pop();

        if (stateStack.empty()) {
            wantExit = true;
            return;
        }
    }

    setState(stateStack.top(), true);
}

int
Gui::addState(State* state) {
    Logger::inst().log_debug("Adding " + state->name + " state to GUI.");

    states.emplace_back(state);
    states[states.size() - 1]->show(false);

    return states.size() - 1;
}

void
Gui::removeState(int state) {
    if (Gui::state == state)
        setState(-1);

    delete states[state];
    states.erase(states.begin() + state);
}

void
Gui::removeStates() {
    for (size_t i = 0; i < states.size(); i++)
        delete states[i];

    states.clear();
}

void
Gui::addOverlay(int stateId) {
    Logger::inst().log_debug("Adding overlay " + getState(stateId).name);

    states[stateId]->show(true);

    overlays.emplace_back(stateId);
}

void
Gui::removeOverlay(int stateId) {
    Logger::inst().log_debug("Removing overlay " + getState(stateId).name);

    states[stateId]->show(false);

    for (auto i = overlays.begin(); i < overlays.end(); i++) {
        if (*i == stateId) {
            overlays.erase(i);
            break;
        }
    }
}

void
Gui::setState(int state, bool back) {
    Logger::inst().log_debug("Setting state to " + (state >= 0 ? getState(state).name : "-1"));

    if (Gui::state >= 0)
        states[Gui::state]->show(false);

    if (state < 0 && !back)
        stateStack.emplace(state);

    for (int i = 0; i < states.size(); i++) {
        if (i == state) {
            states[i]->show(true);

            if (!back)
                stateStack.emplace(i);
        }
    }

    Gui::state = state;
}

void
Gui::setBackground(Background* background) {
    Gui::background = background;
}

void
Gui::exit() {
    wantExit = true;
}

sfg::Desktop&
Gui::getDesktop() {
    return desktop;
}

int
Gui::getState() const {
    return state;
}

State&
Gui::getState(int state) {
    return *states[state];
}

bool
Gui::isWantExit() const {
    return wantExit;
}

}
}
