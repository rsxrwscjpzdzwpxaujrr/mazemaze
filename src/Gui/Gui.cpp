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

#include "Gui.hpp"

#include "GuiState.hpp"

namespace mazemaze {
namespace gui {

Gui::Gui() {

}

Gui::~Gui() = default;

void
Gui::handleEvent(sf::Event event) {
    if (event.type == sf::Event::Resized) {
        for (unsigned int i = 0; i < states.size(); i++) {
            states[i]->center(event);
        }
    }

    desktop.HandleEvent(event);
}

void
Gui::tick(float deltaTime) {
    desktop.Update(deltaTime);

    if (backgroundTickable != nullptr)
        backgroundTickable->tick(deltaTime);
}

void
Gui::render() {
    if (backgroundRenderable != nullptr)
        backgroundRenderable->render();
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
Gui::addState(GuiState* state) {
    states.emplace_back(state);
    states[states.size() - 1]->show(false);

    return states.size() - 1;
}

void
Gui::removeState(int state) {
    states.erase(states.begin() + state);
}

void
Gui::setState(int state, bool back) {
    for (unsigned int i = 0; i < states.size(); i++) {
        if (i == state) {
            states[i]->show(true);

            if (!back)
                stateStack.emplace(i);
        } else {
            states[i]->show(false);
        }
    }

    Gui::state = state;
}

void
Gui::setBackground(ITickable* tickable, IRenderable* renderable) {
    backgroundTickable = tickable;
    backgroundRenderable = renderable;
}

void
Gui::exit() {
    wantExit = true;
}

sfg::Desktop*
Gui::getDesktop() {
    return &desktop;
}

int
Gui::getState() const {
    return state;
}

GuiState*
Gui::getState(int state) {
    return states[state];
}

IRenderable*
Gui::getBackgroundRenderable() const {
    return backgroundRenderable;
}

ITickable*
Gui::getBackgroundTickable() const {
    return backgroundTickable;
}

bool
Gui::isWantExit() const {
    return wantExit;
}

}
}
