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

#include "Progress.hpp"

#include "../../Game.hpp"
#include "../../utils.hpp"

#include "../MainMenu.hpp"

using namespace sfg;

namespace mazemaze {
namespace gui {
namespace states {

void
Progress::init_signals() {
    back_button->GetSignal(Widget::OnLeftClick).Connect([this] {
        if (game) {
            game->get_maze().cancel_generation();
            main_menu.back();
        }
    });
}

Progress::Progress(MainMenu& main_menu) :
        State(main_menu.get_desktop(), "Progress"),
        main_menu(main_menu),
        back_button(Button::Create()),
        maze_size_label(Label::Create()),
        progress_bar(ProgressBar::Create()),
        game(nullptr) {
    reset_text();

    auto button_box         = Box::Create(Box::Orientation::HORIZONTAL);
    auto separator_vertical = Separator::Create(Separator::Orientation::VERTICAL);
    auto window             = Window::Create(Window::Style::BACKGROUND);
    auto window_box         = Box::Create(Box::Orientation::VERTICAL);

    separator_vertical->SetRequisition({0.0f, 20.0f});

    maze_size_label->SetClass("newGameMazeSize");

    window_box->Pack(maze_size_label);
    window_box->Pack(progress_bar);
    window_box->SetSpacing(20.0f);

    window->Add(window_box);

    button_box->Pack(back_button);
    button_box->SetClass("nogap");

    window->SetRequisition({384.0f, 0.0f});

    box = Box::Create(Box::Orientation::VERTICAL);

    box->Pack(window);
    box->Pack(separator_vertical);
    box->Pack(button_box);

    desktop.Add(box);

    init_signals();

    center();
}

void
Progress::tick(void*, float) {
    if (game) {
        if (game->is_loaded()) {
            progress_bar->SetFraction(0);
            main_menu.start_game();
            return;
        }

        progress_bar->SetFraction(game->get_maze().get_generation_progress());
    }
}

void
Progress::reset_text() {
    back_button   ->SetLabel(pgtx("progress", "Cancel"));
    maze_size_label->SetText (pgtx("progress", "Maze generation..."));
}

void
Progress::setGame(Game* game) {
    Progress::game = game;
}

Progress::~Progress() = default;

}
}
}
