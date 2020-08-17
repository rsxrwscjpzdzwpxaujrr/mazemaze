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

#include "Progress.hpp"

#include "../../Game.hpp"
#include "../../utils.hpp"

#include "../MainMenu.hpp"

using namespace sfg;

namespace mazemaze {
namespace gui {
namespace states {

void
Progress::initSignals() {
    backButton->GetSignal(Widget::OnLeftClick).Connect([this] {
        game.getMaze().cancelGeneration();
        mainMenu.back();
    });
}

Progress::Progress(MainMenu& mainMenu, Game& game) :
        State(mainMenu.getDesktop(), "Progress"),
        mainMenu(mainMenu),
        backButton(Button::Create()),
        mazeSizeLabel(Label::Create()),
        progressBar(ProgressBar::Create()),
        game(game) {
    resetText();

    auto buttonBox           = Box::Create(Box::Orientation::HORIZONTAL);
    auto separatorVertical   = Separator::Create(Separator::Orientation::VERTICAL);
    auto window              = Window::Create(Window::Style::BACKGROUND);
    auto windowBox           = Box::Create(Box::Orientation::VERTICAL);

    separatorVertical->SetRequisition({0.0f, 20.0f});

    mazeSizeLabel->SetClass("newGameMazeSize");

    windowBox->Pack(mazeSizeLabel);
    windowBox->Pack(progressBar);
    windowBox->SetSpacing(20.0f);

    progressBar->SetFraction(0.5f);

    window->Add(windowBox);

    buttonBox->Pack(backButton);
    buttonBox->SetClass("nogap");

    window->SetRequisition({384.0f, 0.0f});

    box = Box::Create(Box::Orientation::VERTICAL);

    box->Pack(window);
    box->Pack(separatorVertical);
    box->Pack(buttonBox);

    desktop.Add(box);

    initSignals();

    center();
}

void
Progress::tick(float) {
    if (game.isLoaded()) {
        mainMenu.startGame();
        mainMenu.setState(-1);
    }

    progressBar->SetFraction(game.getMaze().getGenerationProgress());
}

void
Progress::resetText() {
    backButton   ->SetLabel(pgtx("progress", "Cancel"));
    mazeSizeLabel->SetText (pgtx("progress", "Maze generation..."));
}

Progress::~Progress() = default;

}
}
}
