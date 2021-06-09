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

Game::Game(
    gui::MainMenu& main_menu,
    Settings& settings,
    Saver& saver,
    Point2i maze_size
) :
        gui::Background(this, this, nullptr),
        maze(maze_size),
        maze_renderer(0),
        maze_renderers{nullptr},
        player(Pointf(1.5f, 0.0f, 1.5f)),
        settings(settings),
        saver(saver),
        main_menu(main_menu),
        paused(false),
        won(false),
        old_pause_key_state(false),
        time(0.0f),
        loaded(false) {
    Logger::inst().log_debug("Constructor of Game called.");

    saver.set_game(*this);

    maze_renderers[0] = new renderers::Classic(*this);
    maze_renderers[1] = new renderers::Gray(*this);
    maze_renderers[2] = new renderers::Brick(*this);
    maze_renderers[3] = new renderers::NightBrick(*this);

    maze_renderers[maze_renderer]->enable();

    tickable_handler.addTickable(maze_renderers[maze_renderer]);
    tickable_handler.addTickable(&player);

    open_gui();
}

Game::~Game() {
    Logger::inst().log_status("Game closed.");

    main_menu.remove_state(won_state);
    main_menu.remove_state(pause_state);

    maze_renderers[maze_renderer]->disable();

    for (int i = 0; i < 16; i++)
        if (maze_renderers[i] != nullptr)
            delete maze_renderers[i];
}

void
Game::new_game() {
    int seed = gen_seed();

    std::thread gen_thread([this, seed] {
        if (maze.generate(seed)) {
            player.start(maze);
            on_load();
        }
    });

    gen_thread.detach();
}

void
Game::on_load() {
    set_paused(false);
    set_won(false);

    loaded = true;

    Logger::inst().log_status("Game started.");
}

void
Game::tick(void*, float deltaTime) {
    GraphicEngine& graphic_engine = GraphicEngine::inst();

    if     (!sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) &&
            old_pause_key_state &&
            graphic_engine.has_focus())
        set_paused(!paused);

    old_pause_key_state = sf::Keyboard::isKeyPressed(sf::Keyboard::Escape);

    sf::RenderWindow& window = graphic_engine.get_window();
    window.setMouseCursorVisible(paused || won);

    set_renderer(settings.get_renderer());

    tickable_handler.tick(*this, deltaTime);

    if (!(paused || won)) {
        if (time - saver.get_last_save_time() >= settings.get_autosave_time() && settings.get_autosave())
            saver.save();

        if (    static_cast<int>(player.position().x) == maze.exit().x &&
                static_cast<int>(player.position().z) == maze.exit().y)
            set_won(true);

        time += deltaTime;
    }
}

void
Game::open_gui() {
    using namespace gui::states;
    pause_state = main_menu.add_state(new Pause(main_menu, *this));
    won_state   = main_menu.add_state(new Win  (main_menu, *this));
}

void
Game::stop() {
    Logger::inst().log_debug("Game wants exit.");

    if (won)
        saver.delete_save();
    else
        saver.save();

    main_menu.stop_game();
}

void
Game::render() {
    glPushMatrix();

    player.get_camera().setup_perspective();

    player.get_camera().setup_rotation();
    maze_renderers[maze_renderer]->render_sky();

    player.get_camera().setup_translation();
    maze_renderers[maze_renderer]->render();

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
Game::set_paused(bool paused) {
    if (Game::paused != paused && !(paused && Game::won)) {
        Game::paused = paused;

        if (paused) {
            Logger::inst().log_debug("Game paused.");

            main_menu.set_state(pause_state);

            if (settings.get_autosave())
                saver.save();
        } else {
            Logger::inst().log_debug("Game unpaused.");

            main_menu.back_to(-1);
        }
    }
}

void
Game::set_renderer(int id) {
    if (id != maze_renderer) {
        maze_renderers[maze_renderer]->disable();
        tickable_handler.removeTickable(maze_renderers[maze_renderer]);
        maze_renderer = id;
        tickable_handler.addTickable(maze_renderers[maze_renderer]);
        maze_renderers[maze_renderer]->enable();
    }
}

void
Game::set_won(bool won) {
    if (Game::won != won) {
        Game::won = won;

        if (won)
            main_menu.set_state(won_state);
        else
            main_menu.back_to(-1);
    }
}

void
Game::set_time(float time) {
    Game::time = time;
}

bool
Game::is_paused() const {
    return paused;
}

bool
Game::is_won() const {
    return won;
}

bool
Game::is_loaded() const {
    return loaded;
}

float
Game::get_time() const {
    return time;
}

Maze&
Game::get_maze() {
    return maze;
}

Player&
Game::get_player() {
    return player;
}

Camera*
Game::get_camera() {
    return &player.get_camera();
}

Settings&
Game::get_settings() const {
    return settings;
}

unsigned int
Game::gen_seed() {
    using namespace std::chrono;

    auto now = system_clock().now().time_since_epoch();
    return duration_cast<milliseconds>(now).count();
}

}
