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

#include "MainMenu.hpp"

#include <SFML/OpenGL.hpp>
#include <SFML/Window/Event.hpp>

#include <SFGUI/Button.hpp>
#include <SFGUI/Window.hpp>

#include "Game.hpp"
#include "Loader.hpp"

#include "GuiStates/Empty.hpp"
#include "GuiStates/Main.hpp"
#include "GuiStates/Options.hpp"
#include "GuiStates/Pause.hpp"
#include "GuiStates/Win.hpp"
#include "GuiStates/NewGame.hpp"

namespace mazemaze {

MainMenu::MainMenu() {
    desktop.LoadThemeFromFile("data/style.theme");

    setupStarSky();

    states.emplace_back(new menu_states::Main(&desktop, this));
    states.emplace_back(new menu_states::Options(&desktop, this));
    states.emplace_back(new menu_states::Empty(&desktop));
    states.emplace_back(new menu_states::NewGame(&desktop, this));

    setState(0);
}

MainMenu::~MainMenu() = default;

void
MainMenu::handleEvent(sf::Event event) {
    if (event.type == sf::Event::Resized) {
        for (unsigned int i = 0; i < states.size(); i++) {
            states[i]->center(event);
        }
    }

    desktop.HandleEvent(event);
}

void
MainMenu::tick(float deltatime) {
    desktop.Update(deltatime);

    if (backgroundTickable != nullptr)
        backgroundTickable->tick(deltatime);
}

void
MainMenu::render() {
    if (backgroundRenderable != nullptr)
        backgroundRenderable->render();
}

void
MainMenu::back() {
    stateStack.pop();
    setState(stateStack.top(), true);
}

void
MainMenu::backTo(unsigned int destState) {
    while (stateStack.top() != destState) {
        stateStack.pop();

        if (stateStack.empty()) {
            wantExit = true;
            return;
        }
    }

    setState(stateStack.top(), true);
}

void
MainMenu::show(bool show) {
    states[state]->show(show);
}

void
MainMenu::setState(unsigned int state, bool back) {
    for (unsigned int i = 0; i < states.size(); i++) {
        if (i == state) {
            states[i]->show(true);

            if (!back)
                stateStack.emplace(i);
        } else {
            states[i]->show(false);
        }
    }

    MainMenu::state = state;
}

unsigned int
MainMenu::getState() const {
    return state;
}

void
MainMenu::newGame(int mazeWidth, int mazeHeight) {
    Game* game = new Game(this, mazeWidth, mazeHeight);
    game->newGame();

    updateTickableAndRenderable(game);
}

void
MainMenu::resumeGame() {
    Loader loader("sav");
    Game* game = loader.load(this);

    updateTickableAndRenderable(game);
}

void
MainMenu::stopGame() {
    backgroundRenderable = nullptr;
    backgroundTickable = nullptr;

    backTo(0);

    delete backgroundTickable;

    delete states.back();
    states.pop_back();

    delete states.back();
    states.pop_back();

    setupStarSky();
}

void
MainMenu::exit() {
    wantExit = true;
}

bool
MainMenu::isWantExit() {
    return wantExit;
}

inline void
MainMenu::setupStarSky() {
    StarSky* starSky = new StarSky(1024, 600.0f, 1.5f, 2.5f);

    backgroundRenderable = starSky;
    backgroundTickable = starSky;
}

void
MainMenu::updateTickableAndRenderable(Game* game) {
    backgroundRenderable = dynamic_cast<IRenderable*>(game);
    backgroundTickable   = dynamic_cast<ITickable*>(game);

    states.emplace_back(new menu_states::Pause(&desktop, this, game));
    states.back()->show(false);

    states.emplace_back(new menu_states::Win(&desktop, game));
    states.back()->show(false);

    setState(2);
}

}
