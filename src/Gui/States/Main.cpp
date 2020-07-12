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

#include "../MainMenu.hpp"

#include "About.hpp"
#include "NewGame.hpp"
#include "OptionsMenu.hpp"

using namespace sfg;

namespace mazemaze {
namespace gui {
namespace states {

Main::Main(MainMenu& mainMenu, Settings& settings) :
        State(mainMenu.getDesktop()),
        buttonResume( Button::Create(pgtx("main", "Resume"))),
        buttonNewGame(Button::Create(pgtx("main", "New Game"))),
        buttonOptions(Button::Create(pgtx("main", "Options"))),
        buttonAbout(  Button::Create(pgtx("main", "About"))),
        buttonExit(   Button::Create(pgtx("main", "Exit"))),
        settings(settings) {
    initSignals(mainMenu);

    updateButtons(Saver::saveExists(settings));

    OptionsMenu* options = new OptionsMenu(mainMenu, settings);

    newGameState = mainMenu.addState(new NewGame(mainMenu));
    optionsState = mainMenu.addState(options);
    aboutState   = mainMenu.addState(new About(mainMenu, settings));

    mainMenu.setOptionsState(*options, optionsState);
}

Main::~Main() = default;

void
Main::show(bool show) {
    State::show(show);

    if (show)
        updateButtons(Saver::saveExists(settings));
}

void
Main::updateButtons(bool saveExists) {
    desktop.Remove(box);

    box = Box::Create(Box::Orientation::VERTICAL);

    box->SetSpacing(20.0f);

    buttonResume->Show(saveExists);

    box->Pack(buttonResume);
    box->Pack(buttonNewGame);
    box->Pack(buttonOptions);
    box->Pack(buttonAbout);
    box->Pack(buttonExit);

    box->SetRequisition({300.0f, box->GetRequisition().y});

    desktop.Add(box);

    center();
}

void
Main::initSignals(MainMenu& mainMenu) {
    buttonResume->GetSignal(Widget::OnLeftClick).Connect([&mainMenu] {
        mainMenu.resumeGame();
    });

    buttonNewGame->GetSignal(Widget::OnLeftClick).Connect([&mainMenu, this] {
        mainMenu.setState(newGameState);
    });

    buttonOptions->GetSignal(Widget::OnLeftClick).Connect([&mainMenu, this] {
        mainMenu.setState(optionsState);
    });

    buttonAbout->GetSignal(Widget::OnLeftClick).Connect([&mainMenu, this] {
        mainMenu.setState(aboutState);
    });

    buttonExit->GetSignal(Widget::OnLeftClick).Connect([&mainMenu] {
        mainMenu.exit();
    });
}

}
}
}
