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

#include "FpsOverlay.hpp"

#include <SFGUI/Widgets.hpp>

#include "../MainMenu.hpp"

#include "../../Settings.hpp"
#include "../../utils.hpp"

using namespace sfg;

namespace mazemaze {
namespace gui {
namespace states {

FpsOverlay::FpsOverlay(MainMenu& mainMenu, Settings& settings) :
        State(mainMenu.getDesktop(), "FpsOverlay"),
        settings(settings),
        fpsCalculator([this] (float fps) {
            fpsLabel->SetText(fmt("%.2f", fps));
            box->UpdateDrawablePosition();
        }, 0.5f),
        showing(false) {
    auto window = Window::Create(Window::Style::BACKGROUND);

    fpsLabel = Label::Create(L"00,00");
    fpsLabel->SetClass("fps");

    window->Add(fpsLabel);

    box->Pack(window);

    desktop.Add(box);
}

FpsOverlay::~FpsOverlay() = default;

void
FpsOverlay::show(bool show) {
    box->Show(show);

    showing = show;
}

void
FpsOverlay::resetText() {}

void
FpsOverlay::tick(void*, float deltaTime) {
    if (showing)
        fpsCalculator.tick(nullptr, deltaTime);
}

void
FpsOverlay::center() {
    box->UpdateDrawablePosition();
}

}
}
}
