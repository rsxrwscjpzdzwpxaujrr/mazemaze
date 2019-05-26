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

#include "Game.hpp"
#include "Loader.hpp"

#include "GuiStates/Empty.hpp"
#include "GuiStates/Main.hpp"
#include "GuiStates/Options.hpp"
#include "GuiStates/Pause.hpp"
#include "GuiStates/Win.hpp"
#include "GuiStates/NewGame.hpp"

namespace mazemaze {

MainMenu::MainMenu() :
        starSky(new StarSky(1024, 600.0f, 1.5f, 2.5f)) {
    getDesktop()->LoadThemeFromFile("data/style.theme");

    addState(new menu_states::Main   (getDesktop(), this));
    addState(new menu_states::Options(getDesktop(), this));
    addState(new menu_states::Empty  (getDesktop()));
    addState(new menu_states::NewGame(getDesktop(), this));

    setState(0);

    setBackground(starSky, starSky);
}

MainMenu::~MainMenu() {
    delete starSky;

    if (game != nullptr)
        delete game;
}

void
MainMenu::newGame(int mazeWidth, int mazeHeight) {
    game = new Game(this, mazeWidth, mazeHeight);
    game->newGame();

    setupGame();
}

void
MainMenu::resumeGame() {
    Loader loader("sav");
    game = loader.load(this);

    setupGame();
}

void
MainMenu::stopGame() {
    backTo(0);

    delete game;

    delete getState(5);
    delete getState(4);

    removeState(5);
    removeState(4);

    setBackground(starSky, starSky);
}

void
MainMenu::setupGame() {
    setBackground(game, game);

    addState(new menu_states::Pause(getDesktop(), this, game));
    addState(new menu_states::Win  (getDesktop(), game));

    setState(2);
}

}
