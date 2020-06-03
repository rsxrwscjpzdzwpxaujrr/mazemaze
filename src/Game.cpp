/*
 * Copyright (c) 2018-2020, Мира Странная <miraityan2004@gmail.com>
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
#include "MazeRenderers/Brick.hpp"
#include "MazeRenderers/NightBrick.hpp"

namespace mazemaze {

Game::Game(gui::MainMenu& mainMenu, Settings& settings, Saver& saver, int mazeWidth, int mazeHeight) :
        gui::Background(this, this, nullptr),
        maze(mazeWidth, mazeHeight),
        mazeRenderer(0),
        mazeRenderers{nullptr},
        player(1.5f, 0.0f, 1.5f),
        settings(settings),
        saver(saver),
        mainMenu(mainMenu),
        paused(false),
        won(false),
        oldPauseKeyState(false),
        time(0.0f),
        wantExit(false) {
    saver.setGame(*this);

    mazeRenderers[0] = new renderers::Classic(*this);
    mazeRenderers[1] = new renderers::Gray(*this);
    mazeRenderers[2] = new renderers::Brick(*this);
    mazeRenderers[3] = new renderers::NightBrick(*this);

    mazeRenderers[mazeRenderer]->enable();

    openGui();

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
}

void
Game::newGame() {
    maze.generate(static_cast<unsigned int>(std::time(nullptr)));
    player.start(maze);
    onLoad();
}

void
Game::onLoad() {
    setPaused(false);
    setWon(false);
}

void
Game::tick(float deltaTime) {
    GraphicEngine& graphicEngine = GraphicEngine::getInstance();

    if     (!sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) &&
            oldPauseKeyState &&
            graphicEngine.hasFocus())
        setPaused(!paused);

    oldPauseKeyState = sf::Keyboard::isKeyPressed(sf::Keyboard::Escape);

    sf::RenderWindow& window = graphicEngine.getWindow();
    window.setMouseCursorVisible(paused || won);

    setRenderer(settings.getRenderer());

    if (!(paused || won)) {
        player.tick(deltaTime, window, *this);

        if (time - saver.getLastSaveTime() >= settings.getAutosaveTime() && settings.getAutosave())
            saver.save();

        mazeRenderers[mazeRenderer]->tick(deltaTime, player.getX(), player.getZ());

        if (    static_cast<int>(player.getX()) == maze.getExitX() &&
                static_cast<int>(player.getZ()) == maze.getExitY())
            setWon(true);

        time += deltaTime;
    }
}

void
Game::openGui() {
    using namespace gui::states;
    hudState   = mainMenu.addState(new Hud  (mainMenu, settings));
    pauseState = mainMenu.addState(new Pause(mainMenu, *this));
    wonState   = mainMenu.addState(new Win  (mainMenu, *this));
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

        glColor4f(0x20 / 255.0f,
                  0x20 / 255.0f,
                  0x20 / 255.0f,
                  0xD0 / 255.0f);

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
                saver.save();
        } else
            mainMenu.backTo(hudState);
    }
}

void
Game::setWantExit() {
    wantExit = true;

    if (won)
        saver.deleteSave();
    else
        saver.save();

    mainMenu.stopGame();
}

void
Game::setRenderer(int id) {
    if (id != mazeRenderer) {
        mazeRenderers[mazeRenderer]->disable();
        mazeRenderer = id;
        mazeRenderers[mazeRenderer]->enable();

        mazeRenderers[mazeRenderer]->tick(0.0f, player.getX(), player.getY(), true);
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

Settings&
Game::getSettings() {
    return settings;
}

}
