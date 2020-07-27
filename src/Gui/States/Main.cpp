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
#include "../../GraphicEngine.hpp"

#include "../MainMenu.hpp"

#include "About.hpp"
#include "NewGame.hpp"
#include "OptionsMenu.hpp"

using namespace sfg;

namespace mazemaze {
namespace gui {
namespace states {

Main::Main(MainMenu& mainMenu, Settings& settings) :
        State(mainMenu.getDesktop(), "Main"),
        buttonResume( Button::Create(pgtx("main", "Resume"))),
        buttonNewGame(Button::Create(pgtx("main", "New Game"))),
        buttonOptions(Button::Create(pgtx("main", "Options"))),
        buttonAbout(  Button::Create(pgtx("main", "About"))),
        buttonExit(   Button::Create(pgtx("main", "Exit"))),
        settings(settings),
        showing(false) {
    initSignals(mainMenu);

    updateButtons(Saver::saveExists(settings));

    OptionsMenu* options = new OptionsMenu(mainMenu, settings);

    newGameState = mainMenu.addState(new NewGame(mainMenu));
    optionsState = mainMenu.addState(options);
    aboutState   = mainMenu.addState(new About(mainMenu, settings));

    mainMenu.setOptionsState(*options, optionsState);

    buttonAbout->SetClass("verySmall");
}

Main::~Main() = default;

void
Main::show(bool show) {
    State::show(show);

    showing = show;

    buttonAbout->Show(show);

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
    box->Pack(buttonExit);

    desktop.Add(buttonAbout);

    box->SetRequisition({300.0f, box->GetRequisition().y});

    desktop.Add(box);

    center();
}

void
Main::center() {
    sf::Vector2f widgetSize(buttonAbout->GetAllocation().width,
                            buttonAbout->GetAllocation().height);

    sf::Vector2f windowSize =
        static_cast<sf::Vector2f>(GraphicEngine::getInstance().getWindow().getSize());

    buttonAbout->Show(showing && windowSize.x > 640 && windowSize.y > 300);

    sf::Vector2f spacing(24.0f, 24.0f);

    buttonAbout->SetAllocation(sf::FloatRect(
        windowSize - widgetSize - spacing,
        {140.0f, 36.0f}));

    State::center();
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
