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

#include "MainMenu.hpp"

#include <stack>

#include "../Game.hpp"
#include "../Saver.hpp"
#include "../Settings.hpp"
#include "../StarSky.hpp"
#include "../Logger.hpp"
#include "../path_separator.hpp"
#include "../GraphicEngine.hpp"

#include "Background.hpp"

#include "States/Main.hpp"
#include "States/FpsOverlay.hpp"
#include "States/Debug.hpp"

namespace mazemaze {
namespace gui {

MainMenu::MainMenu(Settings& settings) : game(nullptr),
                                         saver(new Saver(settings)),
                                         settings(settings),
                                         fpsShow(false),
                                         debugShow(false) {
    Logger::inst().log_debug("Starting main menu.");

    getDesktop().LoadThemeFromFile("data" PATH_SEPARATOR "style.theme");

    mainState   = addState(new states::Main       (*this, settings));
    fpsState    = addState(new states::FpsOverlay (*this, settings));
    debugState  = addState(new states::Debug      (*this          ));

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

    delete saver;
}

void
MainMenu::onEvent(const sf::Event& event) {
    if (event.type == sf::Event::LostFocus && game != nullptr && game->isLoaded())
        game->setPaused(true);

    if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::F3) {
        if (game && game->isLoaded() && !game->isPaused() && !game->isWon() && !debugShow)
            game->setPaused(true);

        showDebug(!debugShow);
    }
}

Game&
MainMenu::newGame(int mazeWidth, int mazeHeight) {
    game = new Game(*this, settings, *saver, mazeWidth, mazeHeight);
    game->newGame();

    return *game;
}

void
MainMenu::startGame() {
    if (!game)
        return;

    if (!game->isLoaded())
        return;

    setupGame();

    setState(-1);
    game->setPaused(!GraphicEngine::inst().hasFocus());
}

void
MainMenu::resumeGame() {
    game = saver->load(*this);

    startGame();
}

void
MainMenu::stopGame() {
    backTo(mainState);

    delete game;

    game = nullptr;

    setBackground(starSkyBackground);
}

bool
MainMenu::isGameOpen() {
    return game != nullptr;
}

void
MainMenu::showFps(bool show) {
    if (fpsShow != show) {
        if (show)
            addOverlay(fpsState);
        else
            removeOverlay(fpsState);

        fpsShow = show;
    }
}

void
MainMenu::showDebug(bool show) {
    if (debugShow != show) {
        if (show)
            addOverlay(debugState);
        else
            removeOverlay(debugState);

        debugShow = show;
    }
}

int
MainMenu::getOptionsState() const {
    return optionsState;
}

void
MainMenu::setOptionsState(states::OptionsMenu&, int state) {
    optionsState = state;
}

bool
MainMenu::getShowFps() const {
    return fpsShow;
}

bool
MainMenu::getShowDebug() const {
    return debugShow;
}

void
MainMenu::setupGame() {
    setBackground(game);
}

}
}
