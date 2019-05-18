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

#include <cmath>
#include <iostream>
#include <cstdlib>

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Button.hpp>
#include <SFGUI/Desktop.hpp>

#include "Maze.hpp"
#include "MazeRenderer.hpp"
#include "Player.hpp"
#include "StarSky.hpp"
#include "Game.hpp"
#include "MainMenu.hpp"
#include "GraphicEngine.hpp"

using namespace mazemaze;

int
main() {
    const int startWindowWidth = 854;
    const int startWindowHeight = 480;

    sfg::SFGUI sfgui;

    GraphicEngine* graphicEngine = GraphicEngine::getInstance();
    graphicEngine->openWindow(startWindowWidth, startWindowHeight, false);

    float frameStartTime;
    float frameEndTime;
    float frameDeltaTime = 1.0f / 60.0f;

    const float fpsShowInterval = 0.5f;
    float fpsShowSumm = 0.0f;
    int fpsShowCount = 0;

    bool running = true;

    MainMenu mainMenu;

    sf::Clock clock;
    sf::Clock fpsShowClock;

    while (running) {
        frameStartTime = clock.getElapsedTime().asSeconds();

        if (fpsShowClock.getElapsedTime().asSeconds() < fpsShowInterval) {
            fpsShowSumm += 1.0f / frameDeltaTime;
            fpsShowCount++;
        } else {
            std::string s(16, '\0');
            int written = std::snprintf(&s[0], s.size(), "\rFPS: %.2f        ",
                    fpsShowSumm / fpsShowCount);

            s.resize(written);

            std::cout << s << std::flush;

            fpsShowClock.restart();

            fpsShowSumm = 0;
            fpsShowCount = 0;
        }

        sf::RenderWindow* window = graphicEngine->getWindow();

        sf::Event event{};

        while (window->pollEvent(event)) {
            mainMenu.handleEvent(event);

            switch (event.type) {
                case sf::Event::Closed:
                    running = false;
                    break;

                default:
                    break;
            }
        }

        mainMenu.tick(frameDeltaTime);

        graphicEngine->update();
        graphicEngine->setStates();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        mainMenu.render();

        window->resetGLStates();
        sfgui.Display(*window);
        window->display();

        running &= !mainMenu.isWantExit();

        frameEndTime = clock.getElapsedTime().asSeconds();
        frameDeltaTime = frameEndTime - frameStartTime;
    }

    std::cout << std::endl;

    return 0;
}
