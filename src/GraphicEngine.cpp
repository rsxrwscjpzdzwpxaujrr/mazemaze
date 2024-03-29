/*
 * Copyright (c) 2018-2021, Мира Странная <rsxrwscjpzdzwpxaujrr@yahoo.com>
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

#ifdef _WIN32
# include <windows.h>
#endif

#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>

#include <SFGUI/Renderers.hpp>

#include "path_separator.hpp"
#include "Logger.hpp"
#include "utils.hpp"

namespace mazemaze {

GraphicEngine::GraphicEngine() :
        m_window(nullptr),
        old_window_pos(-1, -1),
        old_window_size(854, 480),
        icon_loaded(false),
        running(true),
        need_reopen_event(false),
        m_vsync(false),
        m_focus(true),
        m_max_antialiasing(calc_max_antialiasing()),
        icon(sf::Image()),
        on_set_states([] () {}) {
    auto icon_file = "data" PATH_SEPARATOR "icon.png";

    if (!(icon_loaded = icon.loadFromFile(icon_file))) {
        Logger::inst().log_error(fmt("Can not load icon file \"%s\"", icon_file));
    }

    settings.depthBits = 24;
    settings.stencilBits = 8;

    unwait_key();
}

GraphicEngine::~GraphicEngine() = default;

void
GraphicEngine::open_window() {
    sf::VideoMode video_mode;

    if (m_fullscreen)
        video_mode = sf::VideoMode::getDesktopMode();
    else
        video_mode = sf::VideoMode(old_window_size.x, old_window_size.y);

    open_window(video_mode, m_fullscreen);
}

void
GraphicEngine::open_window(sf::VideoMode video_mode, bool fullscreen) {
    Logger::inst().log_debug(fmt("Main window opening. Size is %dx%d",
                                 video_mode.width,
                                 video_mode.height));

    const sf::String window_name = L"Mazemaze 0.3-git";
    sf::Uint32 style;

    if (fullscreen)
        style = sf::Style::Fullscreen;
    else
        style = sf::Style::Default;

    m_window = new sf::RenderWindow(video_mode, window_name, style, settings);

    if (!fullscreen) {
        if (old_window_pos != sf::Vector2i(-1, -1))
            m_window->setPosition(old_window_pos);

#ifdef _WIN32
        if (old_maximized) {
            ShowWindow(m_window->getSystemHandle(), SW_MAXIMIZE);
            Logger::inst().log_debug("Maximizing window.");
        }
#endif

        GraphicEngine::video_mode = video_mode;
    }

    settings = m_window->getSettings();

    if (icon_loaded) {
        sf::Vector2u icon_size = icon.getSize();
        m_window->setIcon(icon_size.x, icon_size.y, icon.getPixelsPtr());
    }

    set_vsync(m_vsync);
}

void
GraphicEngine::open_window(unsigned int width, unsigned int height, bool fullscreen) {
    sf::VideoMode video_mode = sf::VideoMode(width, height);
    GraphicEngine::open_window(video_mode, fullscreen);
}

void
GraphicEngine::update() {
    sf::Vector2u new_size = m_window->getSize();

    m_window_size = Point2i(new_size.x, new_size.y);

    if (need_reopen) {
        m_window->close();
        delete m_window;

        open_window(video_mode, m_fullscreen);

        need_reopen = false;
        need_reopen_event = true;
    }
}

void
GraphicEngine::set_states() {
//  Logger::inst().log_debug("Setting GL states.");
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glHint(GL_FOG_HINT, GL_FASTEST);

    on_set_states();
}

void
GraphicEngine::loop(sfg::SFGUI& sfgui, gui::MainMenu& main_menu) {
    sf::Clock delta_clock;
    float frame_delta_time = 1.0f / 60.0f;

    sfg::Renderer::Set(sfg::VertexBufferRenderer::Create());

    sfgui.AddCharacterSet(0x20,  0x80);
    sfgui.AddCharacterSet(0xC0,  0x100);
    sfgui.AddCharacterSet(0x400, 0x500);

    while (running) {
        update();
        set_states();

        handle_events(main_menu);

        main_menu.tick(nullptr, frame_delta_time);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        main_menu.render();

        m_window->resetGLStates();
        sfgui.Display(*m_window);
        m_window->display();

        running &= !main_menu.wants_exit();

        frame_delta_time = delta_clock.getElapsedTime().asSeconds();
        delta_clock.restart();
    }
}

void
GraphicEngine::wait_key(std::function<void (const sf::Keyboard::Key)> const& onKey) {
    on_key_waiting = onKey;
}

void
GraphicEngine::unwait_key() {
    on_key_waiting = [] (sf::Keyboard::Key) {};
}

void
GraphicEngine::set_fullscreen(bool fullscreen) {
    if (GraphicEngine::m_fullscreen != fullscreen) {
        GraphicEngine::m_fullscreen = fullscreen;

        if (m_window != nullptr) {
            if (fullscreen) {
                old_window_pos = m_window->getPosition();
                old_window_size = m_window->getSize();
#ifdef _WIN32
                update_old_maximized();
#endif

                video_mode = sf::VideoMode::getDesktopMode();
            } else
                video_mode = sf::VideoMode(old_window_size.x, old_window_size.y);

            need_reopen = true;
        }
    }
}

void
GraphicEngine::set_antialiasing(unsigned int antialiasing) {
    if (antialiasing != settings.antialiasingLevel) {
        if (m_window != nullptr) {
            old_window_pos = m_window->getPosition();
#ifdef _WIN32
            update_old_maximized();
#endif

            if (m_fullscreen)
                video_mode = sf::VideoMode::getDesktopMode();
            else {
                sf::Vector2u windowSize = m_window->getSize();
                video_mode = sf::VideoMode(windowSize.x, windowSize.y);
            }

            need_reopen = true;
        }

        settings.antialiasingLevel = antialiasing;
    }
}

void
GraphicEngine::set_vsync(bool vsync) {
    GraphicEngine::m_vsync = vsync;

    if (m_window != nullptr)
        m_window->setVerticalSyncEnabled(vsync);
}

void
GraphicEngine::set_on_set_states_callback(std::function<void ()> const& on_set_states) {
    GraphicEngine::on_set_states = on_set_states;
}

sf::RenderWindow&
GraphicEngine::window() {
    return *m_window;
}

Point2i
GraphicEngine::window_size() const {
    return m_window_size;
}

unsigned int
GraphicEngine::max_antialiasing() const {
    return m_max_antialiasing;
}

bool
GraphicEngine::fullscreen() const {
    return m_fullscreen;
}

bool
GraphicEngine::vsync() const {
    return m_vsync;
}

bool
GraphicEngine::has_focus() const {
    return m_focus;
}

unsigned int
GraphicEngine::calc_max_antialiasing() {
    sf::ContextSettings settings;
    sf::VideoMode video_mode = sf::VideoMode(16, 16);
    sf::Uint32 style = sf::Style::None;
    settings.antialiasingLevel = 16;

    sf::RenderWindow win(video_mode, sf::String(), style, settings);

    settings = win.getSettings();
    win.close();

    Logger::inst().log_debug(fmt("Calculated max antialiasing is %d.",
                                 settings.antialiasingLevel));

    return settings.antialiasingLevel;
}

#ifdef _WIN32
void
GraphicEngine::update_old_maximized() {
    WINDOWPLACEMENT placement;
    placement.length = sizeof(WINDOWPLACEMENT);

    GetWindowPlacement(m_window->getSystemHandle(), &placement);

    old_maximized = placement.showCmd == SW_MAXIMIZE;
}
#endif

void
GraphicEngine::handle_events(gui::MainMenu& main_menu) {
    sf::Event event;

    while (m_window->pollEvent(event)) {
        main_menu.handle_event(event);

        switch (event.type) {
        case sf::Event::Closed:
            Logger::inst().log_debug("sf::Event::Closed");

            running = false;
            break;

        case sf::Event::KeyReleased:
            on_key_waiting(event.key.code);
            break;

        case sf::Event::GainedFocus:
            Logger::inst().log_debug("sf::Event::GainedFocus");

            m_focus = true;
            break;

        case sf::Event::LostFocus:
            Logger::inst().log_debug("sf::Event::LostFocus");

            m_focus = false;
            break;

        default:
            break;
        }
    }

    if (need_reopen_event) {
        sf::Event event;
        event.type = sf::Event::Resized;

        main_menu.handle_event(event);
        need_reopen_event = false;
    }
}

}
