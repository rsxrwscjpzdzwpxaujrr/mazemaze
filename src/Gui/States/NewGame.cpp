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

#include "NewGame.hpp"
#include "Progress.hpp"

#include "../../utils.hpp"

#include "../MainMenu.hpp"

using namespace sfg;

namespace mazemaze {
namespace gui {
namespace states {

void
NewGame::init_signals(MainMenu& main_menu) {
    start_button->GetSignal(Widget::OnLeftClick).Connect([this, &main_menu] {
        const sf::String text = size_entry->GetText();
        int maze_size = 1;

        if (text.getSize() > 0)
            maze_size = std::stoi(size_entry->GetText().toWideString());

        Game* game = &main_menu.new_game(Point2i(maze_size, maze_size));

        static_cast<Progress&>(main_menu.state(progress_state)).setGame(game);
        main_menu.set_state(progress_state);
    });

    back_button->GetSignal(Widget::OnLeftClick).Connect([&main_menu] {
        main_menu.back();
    });

    size_entry->GetSignal(Entry::OnTextChanged).Connect([this] {
        const sf::String text = size_entry->GetText();
        bool need_old = false;

        if (text.getSize() != 0) {
            need_old = text.getSize() > max_size_chars;

            if (!need_old) {
                for (auto c : text) {
                    if (!std::isdigit(c)) {
                        need_old = true;
                        break;
                    }
                }
            }
        }

        if (need_old) {
            size_entry->SetText(old_text);
            size_entry->SetCursorPosition(old_cursor);
        }

        old_text = size_entry->GetText();
    });
}

NewGame::NewGame(MainMenu& main_menu) :
        State(main_menu.desktop(), "NewGame"),
        back_button(Button::Create()),
        start_button(Button::Create()),
        size_entry(Entry::Create(L"10")),
        maze_size_label(Label::Create()),
        old_text(size_entry->GetText()),
        old_cursor(size_entry->GetCursorPosition()) {
    reset_text();

    auto button_box           = Box::Create(Box::Orientation::HORIZONTAL);
    auto separator_horizontal = Separator::Create(Separator::Orientation::HORIZONTAL);
    auto separator_vertical   = Separator::Create(Separator::Orientation::VERTICAL);
    auto window               = Window::Create(Window::Style::BACKGROUND);
    auto window_box           = Box::Create(Box::Orientation::VERTICAL);

    separator_horizontal->SetRequisition({20.0f, 0.0f});
    separator_vertical->SetRequisition({0.0f, 20.0f});

    size_entry->SetRequisition({10.0f, 10.0f});

    maze_size_label->SetClass("newGameMazeSize");

    window_box->Pack(maze_size_label);
    window_box->Pack(size_entry);
    window_box->SetSpacing(20.0f);

    window->Add(window_box);

    button_box->Pack(back_button);
    button_box->Pack(separator_horizontal);
    button_box->Pack(start_button);
    button_box->SetClass("nogap");

    start_button->SetRequisition({182.0f, 0.0f});
    back_button->SetRequisition({182.0f, 0.0f});

    window->SetRequisition({384.0f, 0.0f});

    box = Box::Create(Box::Orientation::VERTICAL);

    box->Pack(window);
    box->Pack(separator_vertical);
    box->Pack(button_box);

    desktop.Add(box);

    init_signals(main_menu);

    center();

    progress_state = main_menu.add_state(new Progress(main_menu));
}

void
NewGame::tick(void*, float) {
    old_cursor = size_entry->GetCursorPosition();
}

void
NewGame::reset_text() {
    back_button    ->SetLabel(pgtx("new_game", "Back"));
    start_button   ->SetLabel(pgtx("new_game", "Start"));
    maze_size_label->SetText (pgtx("new_game", "Enter maze size"));
}

NewGame::~NewGame() = default;

}
}
}
