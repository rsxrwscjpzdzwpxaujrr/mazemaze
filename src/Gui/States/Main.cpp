/*
 * Copyright (c) 2019-2020, Мира Странная <rsxrwscjpzdzwpxaujrr@yahoo.com>
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

#include "Main.hpp"

#include "../../utils.hpp"
#include "../../Saver.hpp"
#include "../../GraphicEngine.hpp"

#include "../MainMenu.hpp"

#include "About.hpp"
#include "NewGame.hpp"
#include "OptionsMenu.hpp"

using namespace sfg;

namespace mazemaze {
namespace gui {
namespace states {

Main::Main(MainMenu& main_menu, Settings& settings) :
        State(main_menu.desktop(), "Main"),
        resume_button  (Button::Create()),
        new_game_button(Button::Create()),
        options_button (Button::Create()),
        about_button   (Button::Create()),
        exit_button    (Button::Create()),
        settings(settings),
        showing(false) {
    reset_text();

    init_signals(main_menu);

    update_buttons(Saver::save_exists(settings));

    OptionsMenu* options = new OptionsMenu(main_menu, settings);

    new_game_state = main_menu.add_state(new NewGame(main_menu));
    options_state  = main_menu.add_state(options);
    about_state    = main_menu.add_state(new About(main_menu, settings));

    main_menu.set_options_state(*options, options_state);

    about_button->SetClass("verySmall");
}

Main::~Main() = default;

void
Main::show(bool show) {
    State::show(show);

    showing = show;

    about_button->Show(show);

    if (show)
        update_buttons(Saver::save_exists(settings));
}

void
Main::update_buttons(bool save_exists) {
    desktop.Remove(box);

    box = Box::Create(Box::Orientation::VERTICAL);

    box->SetSpacing(20.0f);

    resume_button->Show(save_exists);

    box->Pack(resume_button);
    box->Pack(new_game_button);
    box->Pack(options_button);
    box->Pack(exit_button);

    desktop.Add(about_button);

    box->SetRequisition({300.0f, box->GetRequisition().y});

    desktop.Add(box);

    center();
}

void
Main::center() {
    sf::Vector2f widget_size(about_button->GetAllocation().width,
                             about_button->GetAllocation().height);

    sf::Vector2f window_size =
        static_cast<sf::Vector2f>(GraphicEngine::inst().window().getSize());

    about_button->Show(showing && window_size.x > 640 && window_size.y > 300);

    sf::Vector2f spacing(24.0f, 24.0f);

    about_button->SetAllocation(sf::FloatRect(
        window_size - widget_size - spacing,
        { 140.0f, 36.0f }
    ));

    State::center();
}

void
Main::reset_text() {
    resume_button  ->SetLabel(pgtx("main", "Resume"));
    new_game_button->SetLabel(pgtx("main", "New Game"));
    options_button ->SetLabel(pgtx("main", "Options"));
    about_button   ->SetLabel(pgtx("main", "About"));
    exit_button    ->SetLabel(pgtx("main", "Exit"));
}

void
Main::init_signals(MainMenu& main_menu) {
    resume_button->GetSignal(Widget::OnLeftClick).Connect([&main_menu] {
        main_menu.resume_game();
    });

    new_game_button->GetSignal(Widget::OnLeftClick).Connect([&main_menu, this] {
        main_menu.set_state(new_game_state);
    });

    options_button->GetSignal(Widget::OnLeftClick).Connect([&main_menu, this] {
        main_menu.set_state(options_state);
    });

    about_button->GetSignal(Widget::OnLeftClick).Connect([&main_menu, this] {
        main_menu.set_state(about_state);
    });

    exit_button->GetSignal(Widget::OnLeftClick).Connect([&main_menu] {
        main_menu.exit();
    });
}

}
}
}
