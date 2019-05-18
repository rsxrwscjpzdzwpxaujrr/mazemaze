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

#include <SFGUI/Button.hpp>
#include <SFGUI/Window.hpp>

#include "../Game.hpp"
#include "../MainMenu.hpp"
#include "../GraphicEngine.hpp"

#include "Options.hpp"

#include "Main.hpp"

using namespace sfg;

namespace mazemaze {
namespace menu_states {

inline void
initSignals(Button::Ptr buttonStart, Button::Ptr buttonOptions, Button::Ptr buttonExit,
            MainMenu* mainMenu) {
    buttonStart->GetSignal(Widget::OnLeftClick).Connect([mainMenu] {
        mainMenu->setState(3);
    });

    buttonOptions->GetSignal(Widget::OnLeftClick).Connect([mainMenu] {
        mainMenu->setState(1);
    });

    buttonExit->GetSignal(Widget::OnLeftClick).Connect([mainMenu] {
        mainMenu->exit();
    });
}

Main::Main(Desktop* desktop, MainMenu *mainMenu) : MenuState(desktop) {
    auto buttonStart   = Button::Create(L"Start");
    auto buttonOptions = Button::Create(L"Options");
    auto buttonExit    = Button::Create(L"Exit");
    auto frame         = Window::Create(Window::Style::BACKGROUND);

    frame->Add(buttonOptions);

    box = Box::Create(Box::Orientation::VERTICAL);

    box->Pack(buttonStart);
    box->Pack(buttonOptions);
    box->Pack(buttonExit);

    box->SetSpacing(20.0f);
    box->SetRequisition({300.0f, box->GetRequisition().y});

    desktop->Add(box);

    initSignals(buttonStart, buttonOptions, buttonExit, mainMenu);

    center();
}

Main::~Main() {}

}
}
