/*
 * Copyright (c) 2018-2020, Мира Странная <rsxrwscjpzdzwpxaujrr@yahoo.com>
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

#include <chrono>
#include <thread>

#include <SFML/OpenGL.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include "GraphicEngine.hpp"
#include "Saver.hpp"
#include "Settings.hpp"
#include "Logger.hpp"
#include "MazeRenderer.hpp"

#include "Gui/MainMenu.hpp"

#include "Gui/States/FpsOverlay.hpp"
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
        wantExit(false),
        loaded(false) {
    Logger::inst().log_debug("Constructor of Game called.");

    saver.setGame(*this);

    mazeRenderers[0] = new renderers::Classic(*this);
    mazeRenderers[1] = new renderers::Gray(*this);
    mazeRenderers[2] = new renderers::Brick(*this);
    mazeRenderers[3] = new renderers::NightBrick(*this);

    mazeRenderers[mazeRenderer]->enable();

    tickableHandler.addTickable(mazeRenderers[mazeRenderer]);
    tickableHandler.addTickable(&player);

    openGui();
}

Game::~Game() {
    Logger::inst().log_status("Game closed.");

    mainMenu.removeState(wonState);
    mainMenu.removeState(pauseState);

    mazeRenderers[mazeRenderer]->disable();

    for (int i = 0; i < 16; i++)
        if (mazeRenderers[i] != nullptr)
            delete mazeRenderers[i];
}

void
Game::newGame() {
    int seed = genSeed();

    std::thread genThread([this, seed] {
        if (maze.generate(seed)) {
            player.start(maze);
            onLoad();
        }
    });

    genThread.detach();
}

void
Game::onLoad() {
    setPaused(false);
    setWon(false);

    loaded = true;

    Logger::inst().log_status("Game started.");
}

void
Game::tick(void*, float deltaTime) {
    GraphicEngine& graphicEngine = GraphicEngine::inst();

    if     (!sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) &&
            oldPauseKeyState &&
            graphicEngine.hasFocus())
        setPaused(!paused);

    oldPauseKeyState = sf::Keyboard::isKeyPressed(sf::Keyboard::Escape);

    sf::RenderWindow& window = graphicEngine.getWindow();
    window.setMouseCursorVisible(paused || won);

    setRenderer(settings.getRenderer());

    tickableHandler.tick(*this, deltaTime);

    if (!(paused || won)) {
        if (time - saver.getLastSaveTime() >= settings.getAutosaveTime() && settings.getAutosave())
            saver.save();

        if (    static_cast<int>(player.getX()) == maze.getExitX() &&
                static_cast<int>(player.getZ()) == maze.getExitY())
            setWon(true);

        time += deltaTime;
    }
}

void
Game::openGui() {
    using namespace gui::states;
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
            Logger::inst().log_debug("Game paused.");

            mainMenu.setState(pauseState);

            if (settings.getAutosave())
                saver.save();
        } else {
            Logger::inst().log_debug("Game unpaused.");

            mainMenu.backTo(-1);
        }
    }
}

void
Game::setWantExit() {
    Logger::inst().log_debug("Game wants exit.");

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
        tickableHandler.removeTickable(mazeRenderers[mazeRenderer]);
        mazeRenderer = id;
        tickableHandler.addTickable(mazeRenderers[mazeRenderer]);
        mazeRenderers[mazeRenderer]->enable();
    }
}

void
Game::setWon(bool won) {
    if (Game::won != won) {
        Game::won = won;

        if (won)
            mainMenu.setState(wonState);
        else
            mainMenu.backTo(-1);
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

bool
Game::isLoaded() const {
    return loaded;
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
Game::getSettings() const {
    return settings;
}

unsigned int
Game::genSeed() {
    using namespace std::chrono;

    auto now = system_clock().now().time_since_epoch();
    return duration_cast<milliseconds>(now).count();
}

}
