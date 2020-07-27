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

#include "Pause.hpp"

#include "../../utils.hpp"
#include "../../Game.hpp"

#include "../MainMenu.hpp"

using namespace sfg;

namespace mazemaze {
namespace gui {
namespace states {

Pause::Pause(MainMenu& mainMenu, Game& game) :
        State(mainMenu.getDesktop(), "Pause"),
        buttonResume(Button::Create()),
        buttonOptions(Button::Create()),
        buttonExit(Button::Create()) {
    resetText();

    buttonResume->GetSignal(Widget::OnLeftClick).Connect([&game] {
        game.setPaused(false);
    });

    buttonOptions->GetSignal(Widget::OnLeftClick).Connect([&mainMenu] {
        mainMenu.setState(mainMenu.getOptionsState());
    });

    buttonExit->GetSignal(Widget::OnLeftClick).Connect([&game] {
        game.setWantExit();
    });

    box = Box::Create(Box::Orientation::VERTICAL);

    box->Pack(buttonResume);
    box->Pack(buttonOptions);
    box->Pack(buttonExit);

    box->SetSpacing(20.0f);
    box->SetRequisition({400.0f, box->GetRequisition().y});

    desktop.Add(box);

    center();
}

void
Pause::resetText() {
    buttonResume ->SetLabel(pgtx("pause", "Resume"));
    buttonOptions->SetLabel(pgtx("pause", "Options"));
    buttonExit   ->SetLabel(pgtx("pause", "Exit to main menu"));
}

Pause::~Pause() = default;

}
}
}
