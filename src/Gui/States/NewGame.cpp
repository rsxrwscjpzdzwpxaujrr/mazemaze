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

#include "NewGame.hpp"
#include "Progress.hpp"

#include "../../utils.hpp"

#include "../MainMenu.hpp"

using namespace sfg;

namespace mazemaze {
namespace gui {
namespace states {

void
NewGame::initSignals(MainMenu& mainMenu) {
    startButton->GetSignal(Widget::OnLeftClick).Connect([this, &mainMenu] {
        const sf::String text = sizeEntry->GetText();
        int mazeSize = 1;

        if (text.getSize() > 0)
            mazeSize = std::stoi(sizeEntry->GetText().toWideString());

        Game* game = &mainMenu.newGame(mazeSize, mazeSize);

        static_cast<Progress&>(mainMenu.getState(progressState)).setGame(game);
        mainMenu.setState(progressState);
    });

    backButton->GetSignal(Widget::OnLeftClick).Connect([&mainMenu] {
        mainMenu.back();
    });

    sizeEntry->GetSignal(Entry::OnTextChanged).Connect([this] {
        const sf::String text = sizeEntry->GetText();
        bool needOld = false;

        if (text.getSize() != 0) {
            needOld = text.getSize() > max_size_chars;

            if (!needOld) {
                for (auto c : text) {
                    if (!std::isdigit(c)) {
                        needOld = true;
                        break;
                    }
                }
            }
        }

        if (needOld) {
            sizeEntry->SetText(oldtext);
            sizeEntry->SetCursorPosition(oldcursor);
        }

        oldtext = sizeEntry->GetText();
    });
}

NewGame::NewGame(MainMenu& mainMenu) :
        State(mainMenu.getDesktop(), "NewGame"),
        backButton(Button::Create()),
        startButton(Button::Create()),
        sizeEntry(Entry::Create(L"10")),
        mazeSizeLabel(Label::Create()),
        oldtext(sizeEntry->GetText()),
        oldcursor(sizeEntry->GetCursorPosition()) {
    resetText();

    auto buttonBox           = Box::Create(Box::Orientation::HORIZONTAL);
    auto separatorHorizontal = Separator::Create(Separator::Orientation::HORIZONTAL);
    auto separatorVertical   = Separator::Create(Separator::Orientation::VERTICAL);
    auto window              = Window::Create(Window::Style::BACKGROUND);
    auto windowBox           = Box::Create(Box::Orientation::VERTICAL);

    separatorHorizontal->SetRequisition({20.0f, 0.0f});
    separatorVertical->SetRequisition({0.0f, 20.0f});

    sizeEntry->SetRequisition({10.0f, 10.0f});

    mazeSizeLabel->SetClass("newGameMazeSize");

    windowBox->Pack(mazeSizeLabel);
    windowBox->Pack(sizeEntry);
    windowBox->SetSpacing(20.0f);

    window->Add(windowBox);

    buttonBox->Pack(backButton);
    buttonBox->Pack(separatorHorizontal);
    buttonBox->Pack(startButton);
    buttonBox->SetClass("nogap");

    startButton->SetRequisition({182.0f, 0.0f});
    backButton->SetRequisition({182.0f, 0.0f});

    window->SetRequisition({384.0f, 0.0f});

    box = Box::Create(Box::Orientation::VERTICAL);

    box->Pack(window);
    box->Pack(separatorVertical);
    box->Pack(buttonBox);

    desktop.Add(box);

    initSignals(mainMenu);

    center();

    progressState = mainMenu.addState(new Progress(mainMenu));
}

void
NewGame::tick(void*, float) {
    oldcursor = sizeEntry->GetCursorPosition();
}

void
NewGame::resetText() {
    backButton   ->SetLabel(pgtx("new_game", "Back"));
    startButton  ->SetLabel(pgtx("new_game", "Start"));
    mazeSizeLabel->SetText (pgtx("new_game", "Enter maze size"));
}

NewGame::~NewGame() = default;

}
}
}
