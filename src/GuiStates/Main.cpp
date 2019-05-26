/*
 * Copyright (c) 2019, Мира Странная <miraityan2004@gmail.com>
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

#include <SFGUI/Widgets.hpp>

#include "../Game.hpp"
#include "../MainMenu.hpp"
#include "../GraphicEngine.hpp"

#include "Options.hpp"

using namespace sfg;

namespace mazemaze {
namespace menu_states {

inline void
initSignals(Button::Ptr buttonResume, Button::Ptr buttonNewGame, Button::Ptr buttonOptions,
            Button::Ptr buttonExit, MainMenu* mainMenu) {
    buttonResume->GetSignal(Widget::OnLeftClick).Connect([mainMenu] {
        mainMenu->resumeGame();
    });

    buttonNewGame->GetSignal(Widget::OnLeftClick).Connect([mainMenu] {
        mainMenu->setState(3);
    });

    buttonOptions->GetSignal(Widget::OnLeftClick).Connect([mainMenu] {
        mainMenu->setState(1);
    });

    buttonExit->GetSignal(Widget::OnLeftClick).Connect([mainMenu] {
        mainMenu->exit();
    });
}

Main::Main(Desktop* desktop, MainMenu* mainMenu) : GuiState(desktop) {
    buttonResume  = Button::Create(L"Resume");
    buttonNewGame = Button::Create(L"New Game");
    buttonOptions = Button::Create(L"Options");
    buttonExit    = Button::Create(L"Exit");

    box = Box::Create(Box::Orientation::VERTICAL);

    box->SetSpacing(20.0f);

    desktop->Add(box);

    initSignals(buttonResume, buttonNewGame, buttonOptions, buttonExit, mainMenu);

    updateButtons(fopen("sav", "r"));
}

Main::~Main() = default;

void
Main::show(bool show) {
    GuiState::show(show);

    if (show)
        updateButtons(fopen("sav", "r"));
}

void
Main::updateButtons(bool saveExists) {
    box->RemoveAll();

    if (saveExists)
        box->Pack(buttonResume);

    box->Pack(buttonNewGame);
    box->Pack(buttonOptions);
    box->Pack(buttonExit);

    box->SetRequisition({300.0f, box->GetRequisition().y});

    center();
}

}
}
