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

#include "OptionsControls.hpp"

#include "../MainMenu.hpp"

#include "../../utils.hpp"
#include "../../Settings.hpp"
#include "../../GraphicEngine.hpp"

using namespace sfg;

namespace mazemaze {
namespace gui {
namespace states {

const int OptionsControls::buttonsCount = 4;

OptionsControls::OptionsControls(MainMenu& mainMenu, Settings& settings) :
        Options(mainMenu, settings, "OptionsControls"),
        keyButtons(new Button::Ptr[buttonsCount]),
        keyLabels(new sf::String[buttonsCount] {
            pgtx("options", "Forward"),
            pgtx("options", "Backward"),
            pgtx("options", "Right"),
            pgtx("options", "Left")
        }),
        keyChangeWindow(*this),
        keyControls(new std::string[buttonsCount] {
            "up",
            "down",
            "right",
            "left",
        }) {
    auto sensitivitySlider = Scale::Create(Scrollbar::Orientation::HORIZONTAL);

    sf::Vector2f buttonSize         = {200.0f,  28.0f};
    sf::Vector2f adjustmementBounds = {0.0001f, 0.003f};

    windowBox->Pack(makeOption(pgtx("options", "Mouse sensitivity"), sensitivitySlider));

    sensitivitySlider->SetRequisition(buttonSize);

    sensitivityAdjustement = sensitivitySlider->GetAdjustment();

    sensitivityAdjustement->SetLower(adjustmementBounds.x);
    sensitivityAdjustement->SetUpper(adjustmementBounds.y);

    sensitivityAdjustement->SetMinorStep(
        (adjustmementBounds.y - adjustmementBounds.x) / buttonSize.x);

    for (int i = 0; i < buttonsCount; i++) {
        keyButtons[i] = Button::Create();
        updateKeyButtonLabel(i);

        keyButtons[i]->SetRequisition(buttonSize);
        keyButtons[i]->SetClass("verySmall");
        keyButtons[i]->SetZOrder(0);

        windowBox->Pack(makeOption(keyLabels[i], keyButtons[i]));
    }

    initSignals();

    center();
}

OptionsControls::~OptionsControls() {
    delete [] keyButtons;
    delete [] keyLabels;
    delete [] keyControls;
}

void
OptionsControls::show(bool show) {
    State::show(show);

    if (show)
        sensitivityAdjustement->SetValue(settings.getSensitivity());

    if (!show && keyChangeWindow.isOpened())
        keyChangeWindow.close();
}

void
OptionsControls::tick(float) {
    keyChangeWindow.tick();
}

void
OptionsControls::center() {
    State::center();

    keyChangeWindow.center();
}

void
OptionsControls::updateKeyButtonLabel(int button) {
    std::string control = keyControls[button];

    keyButtons[button]->SetLabel(getKeyName(settings.getKey(control)));
}

void
OptionsControls::initSignals() {
    sensitivityAdjustement->GetSignal(Adjustment::OnChange).Connect([this] () {
        settings.setSensitivity(sensitivityAdjustement->GetValue());
    });

    for (int i = 0; i < buttonsCount; i++) {
        keyButtons[i]->GetSignal(Widget::OnLeftClick).Connect([this, i] () {
            keyChangeWindow.open(i);
        });
    }
}

}
}
}
