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

#include "NewGame.hpp"

#include "../Game.hpp"
#include "../MainMenu.hpp"

using namespace sfg;

namespace mazemaze {
namespace menu_states {

inline void
initSignals(Button::Ptr startButton, Button::Ptr backButton, Entry::Ptr sizeEntry,
            MainMenu* mainMenu) {
    startButton->GetSignal(Widget::OnLeftClick).Connect([mainMenu, sizeEntry] {
        int mazeSize = std::stoi(sizeEntry->GetText().toWideString());

        mainMenu->newGame(mazeSize, mazeSize);
    });

    backButton->GetSignal(Widget::OnLeftClick).Connect([mainMenu] {
        mainMenu->back();
    });

    sizeEntry->GetSignal(Entry::OnTextChanged).Connect([sizeEntry] {
        sf::String text = sizeEntry->GetText();
        sf::String newText = L"";
        bool changed = false;

        for (unsigned int i = 0; i < text.getSize(); i++)
            if (std::isdigit(text[i]))
                newText = newText + text[i];
            else
                changed = true;

        if (text.getSize() == 0 || std::stoi(newText.toWideString()) < 1) {
            newText = L"1";
            changed = true;
        }

        if (changed)
            sizeEntry->SetText(newText);
    });
}

NewGame::NewGame(Desktop* desktop, MainMenu* mainMenu) : MenuState(desktop) {
    auto backButton          = Button::Create(L"Back");
    auto startButton         = Button::Create(L"Start");
    auto buttonBox           = Box::Create(Box::Orientation::HORIZONTAL);
    auto separatorHorizontal = Separator::Create(Separator::Orientation::HORIZONTAL);
    auto separatorVertical   = Separator::Create(Separator::Orientation::VERTICAL);
    auto sizeEntry           = Entry::Create(L"10");
    auto window              = Window::Create(Window::Style::BACKGROUND);
    auto windowBox           = Box::Create(Box::Orientation::VERTICAL);
    auto mazeSizeLabel       = Label::Create(L"Enter maze size");

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

    desktop->Add(box);

    initSignals(startButton, backButton, sizeEntry, mainMenu);

    center();
}

NewGame::~NewGame() = default;

}
}
