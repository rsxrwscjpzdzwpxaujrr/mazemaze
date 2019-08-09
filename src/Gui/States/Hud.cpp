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

#include "Hud.hpp"

#include <SFGUI/Widgets.hpp>

#include "../MainMenu.hpp"

#include "../../utils.hpp"

using namespace sfg;

namespace mazemaze {
namespace gui {
namespace states {

Hud::Hud(MainMenu& mainMenu, Settings& settings) :
        State(mainMenu.getDesktop()),
        settings(settings),
        fpsCalculator([this] (float fps) {
            fpsLabel->SetText(format("%.2f", fps));
            box->UpdateDrawablePosition();
        }, 0.5f) {
    auto window = Window::Create(Window::Style::BACKGROUND);

    fpsLabel = Label::Create(L"00,00");
    fpsLabel->SetClass("fps");

    window->Add(fpsLabel);

    box->Pack(window);

    desktop.Add(box);
}

Hud::~Hud() = default;

void
Hud::show(bool show) {
    box->Show(show && settings.getShowFps());
}

void
Hud::tick(float deltaTime) {
    if (settings.getShowFps())
        fpsCalculator.tick(deltaTime);
}

void
Hud::center() {
    box->UpdateDrawablePosition();
}

}
}
}
