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

OptionsControls::OptionsControls(MainMenu& mainMenu, Settings& settings) :
        Options(mainMenu, settings, "OptionsControls"),
        sensitivitySlider(Scale::Create(Scrollbar::Orientation::HORIZONTAL)),
        sensitivityAdjustement(sensitivitySlider->GetAdjustment()),
        keyChangeWindow(*this),
        sensitivityOpt("", sensitivitySlider),
        keyOpts {
            Option(Button::Create()),
            Option(Button::Create()),
            Option(Button::Create()),
            Option(Button::Create())
        },
        keyControls {
            "up",
            "down",
            "right",
            "left",
        } {
    resetText();

    sf::Vector2f buttonSize         = {200.0f,  28.0f};
    sf::Vector2f adjustmementBounds = {0.0001f, 0.003f};

    windowBox->Pack(sensitivityOpt.toWidget());

    sensitivitySlider->SetRequisition(buttonSize);

    sensitivityAdjustement = sensitivitySlider->GetAdjustment();

    sensitivityAdjustement->SetLower(adjustmementBounds.x);
    sensitivityAdjustement->SetUpper(adjustmementBounds.y);

    sensitivityAdjustement->SetMinorStep(
        (adjustmementBounds.y - adjustmementBounds.x) / buttonSize.x);

    for (int i = 0; i < buttonsCount; i++) {
        keyButtons[i] = std::dynamic_pointer_cast<sfg::Button>(keyOpts[i].getControl());
        updateKeyButtonLabel(i);

        keyButtons[i]->SetRequisition(buttonSize);
        keyButtons[i]->SetClass("verySmall");
        keyButtons[i]->SetZOrder(0);

        windowBox->Pack(keyOpts[i].toWidget());
    }

    initSignals();

    center();
}

OptionsControls::~OptionsControls() = default;

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
OptionsControls::onResetText() {
    keyLabels = {
        pgtx("options", "Forward"),
        pgtx("options", "Backward"),
        pgtx("options", "Right"),
        pgtx("options", "Left")
    };

    for (int i = 0; i < buttonsCount; i++)
        keyOpts[i].changeText(keyLabels[i]);

    sensitivityOpt.changeText(pgtx("options", "Mouse sensitivity"));

    keyChangeWindow.resetText();
}

void
OptionsControls::updateKeyButtonLabel(int button) {
    std::string control = keyControls.at(button);

    keyButtons.at(button)->SetLabel(getKeyName(settings.getKey(control)));
}

void
OptionsControls::initSignals() {
    sensitivityAdjustement->GetSignal(Adjustment::OnChange).Connect([this] () {
        settings.setSensitivity(sensitivityAdjustement->GetValue());
    });

    for (int i = 0; i < buttonsCount; i++) {
        keyButtons.at(i)->GetSignal(Widget::OnLeftClick).Connect([this, i] () {
            keyChangeWindow.open(i);
        });
    }
}

}
}
}
