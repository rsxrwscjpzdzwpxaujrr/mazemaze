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
        State(mainMenu.getDesktop()),
        settings(settings),
        backButton   (Button::Create(pgtx("options", "Back"))),
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
    auto window          = Window::Create(Window::Style::BACKGROUND);
    auto windowBox       = Box::Create(Box::Orientation::VERTICAL);
    auto windowAlignment = Alignment::Create();
    auto scroll          = Scale::Create(Scrollbar::Orientation::HORIZONTAL);

    windowBox->Pack(addToOptionsList(pgtx("options", "Mouse sensitivity"), scroll));

    scroll->SetRequisition({200.0f, 28.0f});

    sensitivityAdjustement = scroll->GetAdjustment();

    sensitivityAdjustement->SetLower(0.0001f);
    sensitivityAdjustement->SetUpper(0.003f);

    sensitivityAdjustement->SetMinorStep((0.003f - 0.0001f) / 200.0f);

    for (int i = 0; i < buttonsCount; i++) {
        keyButtons[i] = Button::Create();
        updateKeyButtonLabel(i);

        keyButtons[i]->SetRequisition({200.0f, 28.0f});
        keyButtons[i]->SetClass("verySmall");
        keyButtons[i]->SetZOrder(0);

        windowBox->Pack(addToOptionsList(keyLabels[i], keyButtons[i]));
    }

    windowAlignment->SetScale({1.0f, 0.0f});
    windowAlignment->SetAlignment({0.0f, 0.0f});
    windowAlignment->Add(windowBox);

    window->Add(windowAlignment);
    window->SetRequisition({512.0f, 300.0f});

    box->SetOrientation(Box::Orientation::VERTICAL);
    box->SetSpacing(20.0f);
    box->Pack(window);
    box->Pack(backButton);

    desktop.Add(box);

    initSignals(mainMenu);

    window->SetState(Window::State::INSENSITIVE);

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

Box::Ptr
OptionsControls::addToOptionsList(const sf::String& label, Widget::Ptr widget) {
    auto alignment1 = Alignment::Create();
    auto alignment2 = Alignment::Create();
    auto box        = Box::Create();

    alignment1->Add(Label::Create(label));

    alignment1->SetScale({0.0f, 0.0f});
    alignment1->SetAlignment({0.0f, 0.5f});

    alignment2->Add(widget);

    alignment2->SetScale({0.0f, 0.0f});
    alignment2->SetAlignment({1.0f, 0.5f});

    box->Pack(alignment1);
    box->Pack(alignment2);
    box->SetClass("options");

    return box;
}

void
OptionsControls::updateKeyButtonLabel(int button) {
    std::string control = keyControls[button];

    keyButtons[button]->SetLabel(getKeyName(settings.getKey(control)));
}

void
OptionsControls::initSignals(MainMenu& mainMenu) {
    sensitivityAdjustement->GetSignal(Adjustment::OnChange).Connect([this] () {
        settings.setSensitivity(sensitivityAdjustement->GetValue());
    });

    backButton->GetSignal(Widget::OnLeftClick).Connect([&mainMenu] () {
        mainMenu.back();
    });

    for (int i = 0; i < buttonsCount; i++) {
        keyButtons[i]->GetSignal(Widget::OnLeftClick).Connect([this, i] () {
            keyChangeWindow.open(i);
        });
    }
}

void
OptionsControls::center() {
    State::center();

    if (keyChangeWindow.isOpened())
        State::center(keyChangeWindow.getWindow());
}

}
}
}
