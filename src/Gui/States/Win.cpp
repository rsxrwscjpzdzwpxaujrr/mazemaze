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

#include "Win.hpp"

#include <string>

#include "../../utils.hpp"
#include "../../Game.hpp"

#include "../MainMenu.hpp"

using namespace sfg;

namespace mazemaze {
namespace gui {
namespace states {

Win::Win(MainMenu& main_menu, Game& game) :
        State(main_menu.get_desktop(), "State"),
        exit_button(Button::Create()),
        win_label(Label::Create()),
        win_note_time_label(Label::Create()),
        win_note_size_label(Label::Create()),
        game(game) {
    reset_text();

    auto win_note_time_alignment = Alignment::Create();
    auto win_note_size_alignment = Alignment::Create();

    win_label->SetClass("win");

    win_note_time_alignment->SetAlignment({0.0f, 0.5f});
    win_note_time_alignment->SetScale({0.0f, 0.0f});
    win_note_time_alignment->Add(win_note_time_label);

    win_note_size_alignment->SetAlignment({0.0f, 0.5f});
    win_note_size_alignment->SetScale({0.0f, 0.0f});
    win_note_size_alignment->Add(win_note_size_label);

    box = Box::Create(Box::Orientation::VERTICAL);

    box->Pack(win_label);
    box->Pack(Separator::Create(Separator::Orientation::VERTICAL));
    box->Pack(win_note_time_alignment);
    box->Pack(win_note_size_alignment);
    box->Pack(Separator::Create(Separator::Orientation::VERTICAL));
    box->Pack(exit_button);

    box->SetSpacing(20.0f);
    box->SetRequisition({400.0f, box->GetRequisition().y});

    desktop.Add(box);

    exit_button->GetSignal(Widget::OnLeftClick).Connect([&game] {
        game.stop();
    });

    center();
}

Win::~Win() = default;

void
Win::show(bool show) {
    box->Show(show);

    if (show)
        update_labels(game);
}

void
Win::reset_text() {
    exit_button->SetLabel(pgtx("win", "Exit to main menu"));
    win_label  ->SetText (pgtx("win", "You won!"));
}

void
Win::update_labels(Game& game) {
    int time  = static_cast<int>(game.get_time());
    int secs  = time % 60;
    int mins  = (time / 60) % 60;
    int hours = (time / (60 * 60)) % 24;
    int days  = time / (60 * 60 * 24);

    sf::String time_string(pgtx("win", "Time: "));

    if (days > 0)
        time_string = time_string + npgtxf("win", "%d day ", "%d days ", days);

    if (hours > 0)
        time_string = time_string + npgtxf("win", "%d hour ", "%d hours ", hours);

    if (mins > 0)
        time_string = time_string + npgtxf("win", "%d min ", "%d mins ", mins);

    if (secs > 0)
        time_string = time_string + npgtxf("win", "%d sec ", "%d secs ", secs);

    win_note_time_label->SetText(time_string);

    Maze& maze = game.get_maze();
    const sf::String maze_size = std::to_wstring((maze.size().x - 1) / 2) +
                                 L"x" +
                                 std::to_wstring((maze.size().y - 1) / 2);

    win_note_size_label->SetText(sf::String(pgtx("win", "Maze size: ")) + maze_size);

    center();
}

}
}
}
