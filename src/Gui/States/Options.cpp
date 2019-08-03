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

#include "Options.hpp"

#include "../../utils.hpp"
#include "../../Settings.hpp"

#include "../MainMenu.hpp"

#include "OptionsGraphics.hpp"
#include "OptionsOther.hpp"

using namespace sfg;

namespace mazemaze {
namespace gui {
namespace states {

void
Options::initSignals(MainMenu& mainMenu) {
    backButton->GetSignal(Widget::OnLeftClick).Connect([&mainMenu] () {
        mainMenu.back();
    });

    graphicsButton->GetSignal(Widget::OnLeftClick).Connect([&mainMenu, this] () {
        mainMenu.setState(graphicsState);
    });

    otherButton->GetSignal(Widget::OnLeftClick).Connect([&mainMenu, this] () {
        mainMenu.setState(otherState);
    });
}

Options::Options(Desktop& desktop, MainMenu& mainMenu, Settings& settings) : State(desktop),
        graphicsButton (Button::Create(pgtx("options", "Graphics"))),
        otherButton    (Button::Create(pgtx("options", "Other"))),
        backButton     (Button::Create(pgtx("options", "Back"))) {
    box->SetOrientation(Box::Orientation::VERTICAL);
    box->SetRequisition({300.0f, box->GetRequisition().y});
    box->SetSpacing(20.0f);

    box->Pack(graphicsButton);
    box->Pack(otherButton);
    box->Pack(backButton);

    desktop.Add(box);

    initSignals(mainMenu);

    center();

    graphicsState = mainMenu.addState(new OptionsGraphics(desktop, mainMenu, settings));
    otherState    = mainMenu.addState(new OptionsOther   (desktop, mainMenu, settings));
}

Options::~Options() = default;

}
}
}
