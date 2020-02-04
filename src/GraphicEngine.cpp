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

#include "GraphicEngine.hpp"

#include <cmath>

#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>

namespace mazemaze {

GraphicEngine::GraphicEngine() :
        window(nullptr),
        oldWindowPos(-1, -1),
        oldWindowSize(854, 480),
        vsync(false),
        focus(true),
        maxAntialiasing(calcMaxAntialiasing()),
        icon(sf::Image()),
        onSetStates([] () {}) {
    icon.loadFromFile("data/icon.png");

    settings.depthBits = 24;
    settings.stencilBits = 8;

    unwaitKey();
}

GraphicEngine::~GraphicEngine() = default;

void
GraphicEngine::openWindow() {
    sf::VideoMode videoMode;

    if (fullscreen)
        videoMode = sf::VideoMode::getDesktopMode();
    else
        videoMode = sf::VideoMode(oldWindowSize.x, oldWindowSize.y);

    openWindow(videoMode, fullscreen);
}

void
GraphicEngine::openWindow(sf::VideoMode videoMode, bool fullscreen) {
    const sf::String windowName = L"Mazemaze 0.2.1-git";
    sf::Uint32 style;

    if (fullscreen)
        style = sf::Style::Fullscreen;
    else
        style = sf::Style::Default;

    window = new sf::RenderWindow(videoMode, windowName, style, settings);

    if (!fullscreen) {
        if (oldWindowPos != sf::Vector2i(-1, -1))
            window->setPosition(oldWindowPos);

        GraphicEngine::videoMode = videoMode;
    }

    settings = window->getSettings();

    sf::Vector2u iconSize = icon.getSize();
    window->setIcon(iconSize.x, iconSize.y, icon.getPixelsPtr());

    setVsync(vsync);
}

void
GraphicEngine::openWindow(unsigned int width, unsigned int height, bool fullscreen) {
    sf::VideoMode videoMode = sf::VideoMode(width, height);
    GraphicEngine::openWindow(videoMode, fullscreen);
}

void
GraphicEngine::update() {
    sf::Vector2u newSize = window->getSize();

    width = newSize.x;
    height = newSize.y;

    if (needReopen) {
        window->close();
        delete window;

        openWindow(videoMode, fullscreen);

        needReopen = false;
    }
}

void
GraphicEngine::setStates() {
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glHint(GL_FOG_HINT, GL_FASTEST);

    onSetStates();
}

void
GraphicEngine::loop(sfg::SFGUI& sfgui, gui::MainMenu& mainMenu) {
    sf::Clock deltaClock;
    float frameDeltaTime = 1.0f / 60.0f;
    bool running = true;

    sfgui.AddCharacterSet(0x20,  0x80);
    sfgui.AddCharacterSet(0x400, 0x500);

    while (running) {
        update();
        setStates();

        sf::Event event{};

        while (window->pollEvent(event)) {
            mainMenu.handleEvent(event);

            switch (event.type) {
                case sf::Event::Closed:
                    running = false;
                    break;

                case sf::Event::KeyReleased:
                    onKeyWaiting(event.key.code);
                    break;

                case sf::Event::GainedFocus:
                    focus = true;
                    break;

                case sf::Event::LostFocus:
                    focus = false;
                    break;

                default:
                    break;
            }
        }

        mainMenu.tick(frameDeltaTime);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        mainMenu.render();

        window->resetGLStates();
        sfgui.Display(*window);
        window->display();

        running &= !mainMenu.isWantExit();

        frameDeltaTime = deltaClock.getElapsedTime().asSeconds();
        deltaClock.restart();
    }
}

void
GraphicEngine::waitKey(std::function<void (const sf::Keyboard::Key)> const& onKey) {
    onKeyWaiting = onKey;
}

void
GraphicEngine::unwaitKey() {
    onKeyWaiting = [] (sf::Keyboard::Key) {};
}

void
GraphicEngine::setFullscreen(bool fullscreen) {
    if (GraphicEngine::fullscreen != fullscreen) {
        GraphicEngine::fullscreen = fullscreen;

        if (window != nullptr) {
            if (fullscreen) {
                oldWindowPos = window->getPosition();
                oldWindowSize = window->getSize();

                videoMode = sf::VideoMode::getDesktopMode();
            } else
                videoMode = sf::VideoMode(oldWindowSize.x, oldWindowSize.y);

            needReopen = true;
        }
    }
}

void
GraphicEngine::setAntialiasing(unsigned int antialiasing) {
    if (antialiasing != settings.antialiasingLevel) {
        if (window != nullptr) {
            oldWindowPos = window->getPosition();

            if (fullscreen)
                videoMode = sf::VideoMode::getDesktopMode();
            else {
                sf::Vector2u windowSize = window->getSize();
                videoMode = sf::VideoMode(windowSize.x, windowSize.y);
            }

            needReopen = true;
        }

        settings.antialiasingLevel = antialiasing;
    }
}

void
GraphicEngine::setVsync(bool vsync) {
    GraphicEngine::vsync = vsync;

    if (window != nullptr)
        window->setVerticalSyncEnabled(vsync);
}

void
GraphicEngine::setOnSetStatesCallback(std::function<void ()> const& onSetStates) {
    GraphicEngine::onSetStates = onSetStates;
}

sf::RenderWindow&
GraphicEngine::getWindow() const {
    return *window;
}

int
GraphicEngine::getWidth() const {
    return width;
}

int
GraphicEngine::getHeight() const {
    return height;
}

unsigned int
GraphicEngine::getMaxAntialiasing() const {
    return maxAntialiasing;
}

bool
GraphicEngine::getFullscreen() const {
    return fullscreen;
}

bool
GraphicEngine::getVsync() const {
    return vsync;
}

bool
GraphicEngine::hasFocus() const {
    return focus;
}

unsigned int
GraphicEngine::calcMaxAntialiasing() {
    sf::ContextSettings settings;
    sf::VideoMode videoMode = sf::VideoMode(16, 16);
    sf::Uint32 style = sf::Style::None;
    settings.antialiasingLevel = 16;

    sf::RenderWindow win(videoMode, sf::String(), style, settings);

    settings = win.getSettings();
    win.close();

    return settings.antialiasingLevel;
}

}
