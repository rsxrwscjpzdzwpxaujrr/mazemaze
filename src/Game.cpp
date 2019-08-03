/*
 * Copyright (c) 2018-2019, Мира Странная <miraityan2004@gmail.com>
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

#include "Game.hpp"

#include <ctime>

#include <SFML/OpenGL.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include "GraphicEngine.hpp"
#include "Saver.hpp"
#include "Settings.hpp"

#include "Gui/MainMenu.hpp"

#include "Gui/States/Hud.hpp"
#include "Gui/States/Pause.hpp"
#include "Gui/States/Win.hpp"

#include "MazeRenderers/Classic.hpp"
#include "MazeRenderers/Gray.hpp"

namespace mazemaze {

Game::Game(gui::MainMenu& mainMenu, Settings& settings, int mazeWidth, int mazeHeight) :
        gui::Background(this, this, nullptr),
        maze(mazeWidth, mazeHeight),
        mazeRenderer(0),
        mazeRenderers{nullptr},
        player(1.5f, 0.0f, 1.5f),
        settings(settings),
        mainMenu(mainMenu),
        lastSaveTime(0.0f),
        paused(false),
        won(false),
        oldPauseKeyState(false),
        time(0.0f),
        wantExit(false) {
    mazeRenderers[0] = new renderers::Classic(*this);
    mazeRenderers[1] = new renderers::Gray(*this);

    mazeRenderers[mazeRenderer]->enable();

    using namespace gui::states;
    hudState   = mainMenu.addState(new Hud  (mainMenu, settings));
    pauseState = mainMenu.addState(new Pause(mainMenu, *this));
    wonState   = mainMenu.addState(new Win  (mainMenu, *this));

    mainMenu.setState(hudState);
}

Game::~Game() {
    mainMenu.removeState(wonState);
    mainMenu.removeState(pauseState);
    mainMenu.removeState(hudState);

    mazeRenderers[mazeRenderer]->disable();

    for (int i = 0; i < 16; i++)
        if (mazeRenderers[i] != nullptr)
            delete mazeRenderers[i];
};

void
Game::newGame() {
    maze.generate(static_cast<unsigned int>(std::time(nullptr)));
    onLoad();
}

void
Game::onLoad() {
    lastSaveTime = time;
    setPaused(false);
    setWon(false);
}

void
Game::tick(float deltaTime) {
    if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) && oldPauseKeyState)
        setPaused(!paused);

    oldPauseKeyState = sf::Keyboard::isKeyPressed(sf::Keyboard::Escape);

    sf::RenderWindow& window = GraphicEngine::getInstance().getWindow();
    window.setMouseCursorVisible(paused || won);

    setRenderer(settings.getRenderer());

    if (!(paused || won)) {
        player.tick(deltaTime, window, maze);

        if (time - lastSaveTime >= settings.getAutosaveTime() && settings.getAutosave()) {
            Saver::getInstance().save(*this);
            lastSaveTime = time;
        }

        mazeRenderers[mazeRenderer]->tick(deltaTime, player.getX(), player.getZ());

        if (    static_cast<int>(player.getX()) == maze.getExitX() &&
                static_cast<int>(player.getZ()) == maze.getExitY())
            setWon(true);

        time += deltaTime;
    }
}

void
Game::render() {
    glPushMatrix();

    player.getCamera().setupPerspective();

    player.getCamera().setupRotation();

    player.getCamera().setupTranslation();
    mazeRenderers[mazeRenderer]->render();

    glPopMatrix();

    if (paused || won) {
        glDisable(GL_DEPTH_TEST);

        glBegin(GL_QUADS);

        glColor4f(0x2D / 255.0f,
                  0x2D / 255.0f,
                  0x2D / 255.0f,
                  0xB0 / 255.0f);

        glVertex3f(100.0f, -100.0f, -1.0f);
        glVertex3f(100.0f, 100.0f, -1.0f);
        glVertex3f(-100.0f, 100.0f, -1.0f);
        glVertex3f(-100.0f, -100.0f, -1.0f);

        glEnd();

        glDisable(GL_DEPTH_TEST);
    }
}

void
Game::setPaused(bool paused) {
    if (Game::paused != paused && !(paused && Game::won)) {
        Game::paused = paused;

        if (paused) {
            mainMenu.setState(pauseState);

            if (settings.getAutosave())
                Saver::getInstance().save(*this);
        } else
            mainMenu.backTo(hudState);
    }
}

void
Game::setWantExit() {
    wantExit = true;

    if (won)
        Saver::getInstance().deleteSave(*this);
    else
        Saver::getInstance().save(*this);

    mainMenu.stopGame();
}

void
Game::setRenderer(int id) {
    if (id != mazeRenderer) {
        mazeRenderers[mazeRenderer]->disable();
        mazeRenderer = id;
        mazeRenderers[mazeRenderer]->enable();

        mazeRenderers[mazeRenderer]->tick(0.0f, player.getX(), player.getY());
    }
}

void
Game::setWon(bool won) {
    if (Game::won != won) {
        Game::won = won;

        if (won)
            mainMenu.setState(wonState);
        else
            mainMenu.backTo(hudState);
    }
}

void
Game::setTime(float time) {
    Game::time = time;
}

bool
Game::isPaused() const {
    return paused;
}

bool
Game::isWon() const {
    return won;
}

bool
Game::isWantExit() const {
    return wantExit;
}

float
Game::getTime() const {
    return time;
}

Maze&
Game::getMaze() {
    return maze;
}

Player&
Game::getPlayer() {
    return player;
}

Camera*
Game::getCamera() {
    return &player.getCamera();
}

}
