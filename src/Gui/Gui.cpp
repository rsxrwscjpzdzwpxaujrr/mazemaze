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
             wantExit(false) {}

Gui::~Gui() {
    removeStates();
}

void
Gui::handleEvent(const sf::Event& event) {
    if (event.type == sf::Event::Resized) {
        for (auto state : states)
            state->center();
    }

    onEvent(event);

    desktop.HandleEvent(event);
}

void
Gui::onEvent(const sf::Event&) {

}

void
Gui::tick(void* _, float deltaTime) {
    desktop.Update(deltaTime);

    if (state > 0)
        states.at(state)->tick(deltaTime);

    if (background != nullptr)
        background->tick(_, deltaTime);

    for (auto overlay : overlays) {
        states.at(overlay)->tick(deltaTime);
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
    states.at(states.size() - 1)->show(false);

    return states.size() - 1;
}

void
Gui::removeState(int state) {
    if (Gui::state == state)
        setState(-1);

    delete states.at(state);
    states.erase(states.begin() + state);
}

void
Gui::removeStates() {
    for (auto state : states)
        delete state;

    states.clear();
}

void
Gui::resetText() {
    Logger::inst().log_debug("Resetting text.");

    for (auto state : states)
        state->resetText();
}

void
Gui::addOverlay(int stateId) {
    Logger::inst().log_debug("Adding overlay " + getState(stateId).name);

    states.at(stateId)->show(true);

    overlays.emplace_back(stateId);
}

void
Gui::removeOverlay(int stateId) {
    Logger::inst().log_debug("Removing overlay " + getState(stateId).name);

    states.at(stateId)->show(false);

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
    return *states.at(state);
}

bool
Gui::isWantExit() const {
    return wantExit;
}

}
}
