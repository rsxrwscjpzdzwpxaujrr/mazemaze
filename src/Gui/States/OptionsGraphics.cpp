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

#include "OptionsGraphics.hpp"

#include "../MainMenu.hpp"

#include "../../utils.hpp"
#include "../../Settings.hpp"

using namespace sfg;

namespace mazemaze {
namespace gui {
namespace states {

Box::Ptr
OptionsGraphics::addToOptionsList(const sf::String& label, Widget::Ptr widget) {
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
OptionsGraphics::initSignals(MainMenu& mainMenu) {
    fullscreenCheck->GetSignal(Widget::OnLeftClick).Connect([this] () {
        settings.setFullscreen(fullscreenCheck->IsActive());
    });

    antialiasingCombo->GetSignal(ComboBox::OnSelect).Connect([this] () {
        int item = antialiasingCombo->GetSelectedItem();
        unsigned int antialiasing = 1;

        for (int i = 0; i < item; i++)
            antialiasing *= 2;

        if (antialiasing == 1)
            antialiasing = 0;

        settings.setAntialiasing(antialiasing);
    });

    vsyncCheck->GetSignal(Widget::OnLeftClick).Connect([this] () {
        settings.setVsync(vsyncCheck->IsActive());
    });

    backButton->GetSignal(Widget::OnLeftClick).Connect([&mainMenu] () {
        mainMenu.back();
    });

    styleCombo->GetSignal(ComboBox::OnSelect).Connect([this] () {
        settings.setRenderer(styleCombo->GetSelectedItem());
    });
}

void
OptionsGraphics::initAntialiasingCombo() {
    int maxAntialiasing = settings.getMaxAntialiasing();

    antialiasingCombo->AppendItem(pgtx("options", "No"));

    for (int i = 2; i <= maxAntialiasing; i *= 2)
        antialiasingCombo->AppendItem(format("%dx", i));
}

void
OptionsGraphics::initOptions() {
    fullscreenCheck->SetActive(settings.getFullscreen());

    initAntialiasingCombo();

    unsigned int antialiasing = settings.getAntialiasing();

    if (antialiasing == 0)
        antialiasing = 1;

    for (unsigned int i = 0, j = 1; j <= antialiasing; i++, j *= 2)
        if (j == antialiasing) {
            antialiasingCombo->SelectItem(i);
            break;
        }

    vsyncCheck->SetActive(settings.getVsync());

    styleCombo->AppendItem(pgtx("options", "Classic"));
    styleCombo->AppendItem(pgtx("options", "Gray"));
    styleCombo->AppendItem(pgtx("options", "Brick"));
    styleCombo->SelectItem(settings.getRenderer());
}

OptionsGraphics::OptionsGraphics(MainMenu& mainMenu, Settings& settings) :
        State(mainMenu.getDesktop()),
        settings(settings),
        backButton       (Button::Create(pgtx("options", "Back"))),
        fullscreenCheck  (CheckButton::Create(L"")),
        vsyncCheck       (CheckButton::Create(L"")),
        antialiasingCombo(ComboBox::Create()),
        styleCombo       (ComboBox::Create()) {
    auto window             = Window::Create(Window::Style::BACKGROUND);
    auto windowBox          = Box::Create(Box::Orientation::VERTICAL);
    auto windowAlignment    = Alignment::Create();

    windowBox->Pack(addToOptionsList(pgtx("options", "Fullscreen"), fullscreenCheck));
    windowBox->Pack(addToOptionsList(pgtx("options", "Antialiasing"), antialiasingCombo));
    windowBox->Pack(addToOptionsList(pgtx("options", "V-Sync"), vsyncCheck));
    windowBox->Pack(addToOptionsList(pgtx("options", "Style"), styleCombo));

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
    initOptions();

    center();
}

OptionsGraphics::~OptionsGraphics() = default;

}
}
}
