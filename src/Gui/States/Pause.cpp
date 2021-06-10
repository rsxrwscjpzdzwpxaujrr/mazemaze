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

#include "Pause.hpp"

#include "../../utils.hpp"
#include "../../Game.hpp"

#include "../MainMenu.hpp"

using namespace sfg;

namespace mazemaze {
namespace gui {
namespace states {

Pause::Pause(MainMenu& main_menu, Game& game) :
        State(main_menu.desktop(), "Pause"),
        resume_button(Button::Create()),
        options_button(Button::Create()),
        exit_button(Button::Create()) {
    reset_text();

    resume_button->GetSignal(Widget::OnLeftClick).Connect([&game] {
        game.set_paused(false);
    });

    options_button->GetSignal(Widget::OnLeftClick).Connect([&main_menu] {
        main_menu.set_state(main_menu.options_state());
    });

    exit_button->GetSignal(Widget::OnLeftClick).Connect([&game] {
        game.stop();
    });

    box = Box::Create(Box::Orientation::VERTICAL);

    box->Pack(resume_button);
    box->Pack(options_button);
    box->Pack(exit_button);

    box->SetSpacing(20.0f);
    box->SetRequisition({400.0f, box->GetRequisition().y});

    desktop.Add(box);

    center();
}

void
Pause::reset_text() {
    resume_button ->SetLabel(pgtx("pause", "Resume"));
    options_button->SetLabel(pgtx("pause", "Options"));
    exit_button   ->SetLabel(pgtx("pause", "Exit to main menu"));
}

Pause::~Pause() = default;

}
}
}
