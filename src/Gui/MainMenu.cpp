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

#include "../Game.hpp"
#include "../Saver.hpp"
#include "../Settings.hpp"
#include "../StarSky.hpp"

#include "Background.hpp"

#include "States/Main.hpp"

namespace mazemaze {
namespace gui {

MainMenu::MainMenu(Settings& settings) : game(nullptr),
                                         settings(settings) {
    getDesktop().LoadThemeFromFile("data/style.theme");

    //Костыль
    const sf::String str(L"АӘБВГҐҒДЕЁЄЖЗИЇЙКҚЛМНҢОӨПРСТУҰҮФХҺЦЧШЩЪЫІЬЭEЮЯ\
                           аәбвгґғдеёєжзиїйкқлмнңоөпрстуұүфхһцчшщъыіьэeюя");

    sfg::Button::Create(str);
    sfg::Label::Create(str);

    mainState = addState(new states::Main(getDesktop(), *this, settings));
    setState(mainState);

    StarSky* starsky = new StarSky(1024, 600.0f, 0.0f, 0.0f);

    starSkyBackground = new Background(starsky, starsky,
                                       new Camera(0.0f,  0.0f,  0.0f,
                                                  -1.5f, -2.5f, 0.0f,
                                                  110.0, 10.0,  100.0));

    setBackground(starSkyBackground);
}

MainMenu::~MainMenu() {
    delete starSkyBackground->getTickable();
    delete starSkyBackground->getCamera();
    delete starSkyBackground;

    if (game != nullptr)
        delete game;
}

void
MainMenu::onEvent(sf::Event event) {
    if (event.type == sf::Event::LostFocus && game != nullptr)
        game->setPaused(true);
}

void
MainMenu::newGame(int mazeWidth, int mazeHeight) {
    game = new Game(*this, settings, mazeWidth, mazeHeight);
    game->newGame();

    setupGame();
}

void
MainMenu::resumeGame() {
    game = Saver::getInstance().load(*this, settings);

    setupGame();
}

void
MainMenu::stopGame() {
    backTo(mainState);

    delete game;

    game = nullptr;

    setBackground(starSkyBackground);
}

int
MainMenu::getOptionsState() const {
    return optionsState;
}

void
MainMenu::setOptionsState(states::Options& options, int state) {
    optionsState = state;
}

void
MainMenu::setupGame() {
    setBackground(game);
}

}
}
