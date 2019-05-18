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

#include <SFGUI/Separator.hpp>
#include <SFGUI/Alignment.hpp>
#include <SFGUI/Button.hpp>

#include "../Game.hpp"
#include "../GraphicEngine.hpp"
#include "../MainMenu.hpp"

#include "Win.hpp"

using namespace sfg;

namespace mazemaze {
namespace menu_states {

Win::Win(Desktop* desktop, Game* game) : MenuState (desktop) {
    auto buttonExit           = Button::Create(L"Exit to main menu");
    auto label                = Label::Create(L"You won!");
    auto winNoteTimeAlignment = Alignment::Create();
    auto winNoteSizeAlignment = Alignment::Create();

    Win::game = game;

    label->SetClass("win");

    winNoteTimeLabel = Label::Create();
    winNoteSizeLabel = Label::Create();

    winNoteTimeAlignment->SetAlignment({0.0f, 0.5f});
    winNoteTimeAlignment->SetScale({0.0f, 0.0f});
    winNoteTimeAlignment->Add(winNoteTimeLabel);

    winNoteSizeAlignment->SetAlignment({0.0f, 0.5f});
    winNoteSizeAlignment->SetScale({0.0f, 0.0f});
    winNoteSizeAlignment->Add(winNoteSizeLabel);

    box = Box::Create(Box::Orientation::VERTICAL);

    box->Pack(label);
    box->Pack(Separator::Create(Separator::Orientation::VERTICAL));
    box->Pack(winNoteTimeAlignment);
    box->Pack(winNoteSizeAlignment);
    box->Pack(Separator::Create(Separator::Orientation::VERTICAL));
    box->Pack(buttonExit);

    box->SetSpacing(20.0f);
    box->SetRequisition({400.0f, box->GetRequisition().y});

    desktop->Add(box);

    buttonExit->GetSignal(Widget::OnLeftClick).Connect([game] {
        game->setWantExit();
    });

    center();
}

Win::~Win() {}

void
Win::show(bool show) {
    box->Show(show);

    if (show)
        updateLabels(game);
}

void
Win::updateLabels(Game* game) {
    Maze* maze = game->getMaze();
    sf::String mazeSize =
            std::to_wstring((maze->getWidth() - 1) / 2) +
            L"x" +
            std::to_wstring((maze->getHeight() - 1) / 2);

    int time = static_cast<int>(game->getTime());

    winNoteTimeLabel->SetText(L"Ingame time: " + std::to_wstring(time) + L" sec");
    winNoteSizeLabel->SetText((sf::String(L"Maze size: ") + mazeSize));

    center();
}

}
}
