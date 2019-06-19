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

#include "GraphicEngine.hpp"

#include <cmath>

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

namespace mazemaze {

GraphicEngine::GraphicEngine() :
        oldWindowPos(-1, -1),
        oldWindowSize(0, 0),
        maxAntialiasing(-1),
        vsync(false),
        icon(new sf::Image()) {
    icon->loadFromFile("data/icon.png");

    settings.depthBits = 24;
    settings.stencilBits = 8;
    getMaxAntialiasing();
}

GraphicEngine::~GraphicEngine() {
    delete icon;
}

void
GraphicEngine::openWindow(sf::VideoMode videoMode, bool fullscreen) {
    const sf::String windowName = L"Mazemaze 0.2-dev";
    sf::Uint32 style;

    if (fullscreen)
        style = sf::Style::Fullscreen;
    else
        style = sf::Style::Default;

    if (fullscreen) {
        window = new sf::RenderWindow(videoMode, windowName, style, settings);
    } else {
        window = new sf::RenderWindow(videoMode, windowName, style, settings);

        if (oldWindowPos != sf::Vector2i(-1, -1))
            window->setPosition(oldWindowPos);

        GraphicEngine::videoMode = videoMode;
    }

    settings = window->getSettings();

    sf::Vector2u iconSize = icon->getSize();
    window->setIcon(iconSize.x, iconSize.y, icon->getPixelsPtr());

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
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void
GraphicEngine::setFullscreen(bool fullscreen) {
    if (GraphicEngine::fullscreen != fullscreen) {
        GraphicEngine::fullscreen = fullscreen;

        if (fullscreen) {
            oldWindowPos = window->getPosition();
            oldWindowSize = window->getSize();

            videoMode = sf::VideoMode::getDesktopMode();
        } else
            videoMode = sf::VideoMode(oldWindowSize.x, oldWindowSize.y);

        needReopen = true;
    }
}

void
GraphicEngine::setAntialiasing(unsigned int antialiasing) {
    if (antialiasing != settings.antialiasingLevel) {
        oldWindowPos = window->getPosition();

        if (fullscreen)
            videoMode = sf::VideoMode::getDesktopMode();
        else {
            sf::Vector2u windowSize = window->getSize();
            videoMode = sf::VideoMode(windowSize.x, windowSize.y);
        }

        settings.antialiasingLevel = antialiasing;
        needReopen = true;
    }
}

void
GraphicEngine::setVsync(bool vsync) {
    GraphicEngine::vsync = vsync;

    window->setVerticalSyncEnabled(vsync);
}

sf::RenderWindow*
GraphicEngine::getWindow() const {
    return window;
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
GraphicEngine::getMaxAntialiasing() {
    if (maxAntialiasing <= 0) {
        sf::ContextSettings settings;
        sf::VideoMode videoMode = sf::VideoMode(16, 16);
        sf::Uint32 style = sf::Style::None;
        settings.antialiasingLevel = 16;

        sf::RenderWindow win(videoMode, sf::String(), style, settings);

        settings = win.getSettings();
        win.close();

        maxAntialiasing = settings.antialiasingLevel;
    }

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

}
