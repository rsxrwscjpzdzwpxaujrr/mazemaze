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
                                         fps_show(false),
                                         debug_show(false) {
    Logger::inst().log_debug("Starting main menu.");

    get_desktop().LoadThemeFromFile("data" PATH_SEPARATOR "style.theme");

    main_state   = add_state(new states::Main       (*this, settings));
    fps_state    = add_state(new states::FpsOverlay (*this, settings));
    debug_state  = add_state(new states::Debug      (*this          ));

    set_state(main_state);

    StarSky* star_sky = new StarSky(1024, 600.0f, Rotation());

    star_sky_background = new Background(
        star_sky, star_sky,
        new Camera(Pointf  ( 0.0f,  0.0f, 0.0f),
                   Rotation(-1.5f, -2.5f, 0.0f),
                   110.0,  10.0, 100.0)
    );

    set_background(star_sky_background);
}

MainMenu::~MainMenu() {
    delete star_sky_background->get_tickable();
    delete star_sky_background->get_camera();
    delete star_sky_background;

    if (game != nullptr)
        delete game;

    delete saver;
}

void
MainMenu::on_event(const sf::Event& event) {
    if (event.type == sf::Event::LostFocus && game != nullptr && game->is_loaded())
        game->set_paused(true);

    if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::F3) {
        if (game && game->is_loaded() && !game->is_paused() && !game->is_won() && !debug_show)
            game->set_paused(true);

        show_debug(!debug_show);
    }
}

Game&
MainMenu::new_game(Point2i maze_size) {
    game = new Game(*this, settings, *saver, maze_size);
    game->new_game();

    return *game;
}

void
MainMenu::start_game() {
    if (!game)
        return;

    if (!game->is_loaded())
        return;

    setup_game();

    set_state(-1);
    game->set_paused(!GraphicEngine::inst().has_focus());
}

void
MainMenu::resume_game() {
    game = saver->load(*this);

    start_game();
}

void
MainMenu::stop_game() {
    back_to(main_state);

    delete game;

    game = nullptr;

    set_background(star_sky_background);
}

bool
MainMenu::is_game_open() {
    return game != nullptr;
}

void
MainMenu::show_fps(bool show) {
    if (fps_show != show) {
        if (show)
            add_overlay(fps_state);
        else
            remove_overlay(fps_state);

        fps_show = show;
    }
}

void
MainMenu::show_debug(bool show) {
    if (debug_show != show) {
        if (show)
            add_overlay(debug_state);
        else
            remove_overlay(debug_state);

        debug_show = show;
    }
}

int
MainMenu::get_options_state() const {
    return options_state;
}

void
MainMenu::set_options_state(states::OptionsMenu&, int state) {
    options_state = state;
}

bool
MainMenu::get_show_fps() const {
    return fps_show;
}

bool
MainMenu::get_show_debug() const {
    return debug_show;
}

void
MainMenu::setup_game() {
    set_background(game);
}

}
}
