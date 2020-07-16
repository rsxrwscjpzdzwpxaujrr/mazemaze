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

#include "OptionsMenu.hpp"

#include "../../utils.hpp"
#include "../../Settings.hpp"

#include "../MainMenu.hpp"

#include "OptionsGraphics.hpp"
#include "OptionsControls.hpp"
#include "OptionsOther.hpp"

using namespace sfg;

namespace mazemaze {
namespace gui {
namespace states {

void
OptionsMenu::initSignals(MainMenu& mainMenu) {
    backButton->GetSignal(Widget::OnLeftClick).Connect([&mainMenu] () {
        mainMenu.back();
    });

    graphicsButton->GetSignal(Widget::OnLeftClick).Connect([&mainMenu, this] () {
        mainMenu.setState(graphicsState);
    });

    controlsButton->GetSignal(Widget::OnLeftClick).Connect([&mainMenu, this] () {
        mainMenu.setState(controlsState);
    });

    otherButton->GetSignal(Widget::OnLeftClick).Connect([&mainMenu, this] () {
        mainMenu.setState(otherState);
    });
}

OptionsMenu::OptionsMenu(MainMenu& mainMenu, Settings& settings) :
        State(mainMenu.getDesktop(), "OptionsMenu"),
        graphicsButton (Button::Create(pgtx("options", "Graphics"))),
        controlsButton (Button::Create(pgtx("options", "Controls"))),
        otherButton    (Button::Create(pgtx("options", "Other"))),
        backButton     (Button::Create(pgtx("options", "Back"))) {
    box->SetOrientation(Box::Orientation::VERTICAL);
    box->SetRequisition({300.0f, box->GetRequisition().y});
    box->SetSpacing(20.0f);

    box->Pack(graphicsButton);
    box->Pack(controlsButton);
    box->Pack(otherButton);
    box->Pack(backButton);

    desktop.Add(box);

    initSignals(mainMenu);

    center();

    graphicsState = mainMenu.addState(new OptionsGraphics(mainMenu, settings));
    controlsState = mainMenu.addState(new OptionsControls(mainMenu, settings));
    otherState    = mainMenu.addState(new OptionsOther   (mainMenu, settings));
}

OptionsMenu::~OptionsMenu() = default;

}
}
}
