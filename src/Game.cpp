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

#include <ctime>

#include <SFML/OpenGL.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include "GraphicEngine.hpp"
#include "MainMenu.hpp"

#include "Game.hpp"

namespace mazemaze {

Game::Game(MainMenu* mainMenu, int mazeWidth, int mazeHeight) : maze(mazeWidth, mazeHeight),
                                                                player(1.5f, 0.5f, 1.5f),
                                                                starSky(1024, 0.0f, 1.5f, 0.7f) {
    maze.generate(static_cast<uint>(std::time(nullptr)));
    mazeRenderer.update(&maze);
    setPaused(false);
    setWon(false);

    Game::mainMenu = mainMenu;
}

Game::~Game() = default;

void
Game::tick(float deltaTime) {
    if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) && oldPauseKeyState)
        setPaused(!paused);

    oldPauseKeyState = sf::Keyboard::isKeyPressed(sf::Keyboard::Escape);

    sf::RenderWindow* window = GraphicEngine::getInstance()->getWindow();
    window->setMouseCursorVisible(paused || won);

    if (!(paused || won)) {
        player.tick(deltaTime, window, &maze);

        if (    static_cast<int>(player.getX()) == maze.getExitX() &&
                static_cast<int>(player.getZ()) == maze.getExitY())
            setWon(true);

        time += deltaTime;
        starSky.setTime(time);
    }
}

void
Game::render() {
    glPushMatrix();

    player.setCameraRotation();

    starSky.render();
    player.setCameraTranslation();

    mazeRenderer.render();

    glPopMatrix();

    if (paused || won) {
        glBegin(GL_QUADS);

        glColor4f(0x2D / 255.0f,
                  0x2D / 255.0f,
                  0x2D / 255.0f,
                  0xB0 / 255.0f);

        glVertex3f(1.0f, -1.0f, -0.002f);
        glVertex3f(1.0f, 1.0f, -0.002f);
        glVertex3f(-1.0f, 1.0f, -0.002f);
        glVertex3f(-1.0f, -1.0f, -0.002f);

        glEnd();
    }
}

void
Game::setPaused(bool paused) {
    if (Game::paused != paused) {
        Game::paused = paused;

        if (paused)
            mainMenu->setState(4);
        else
            mainMenu->backTo(2);
    }
}

void
Game::setWantExit() {
    wantExit = true;

    mainMenu->stopGame();
}

void
Game::setWon(bool won) {
    if (Game::won != won) {
        Game::won = won;

        if (won)
            mainMenu->setState(5);
        else
            mainMenu->backTo(2);
    }
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

Maze*
Game::getMaze() {
    return &maze;
}

}
