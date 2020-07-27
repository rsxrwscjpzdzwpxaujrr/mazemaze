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

#include "Options.hpp"

#include "../../utils.hpp"
#include "../../Settings.hpp"

#include "../MainMenu.hpp"

#include "OptionsGraphics.hpp"
#include "OptionsControls.hpp"
#include "OptionsOther.hpp"

using namespace sfg;

namespace mazemaze {
namespace gui {
namespace states {

void
Options::initSignals(MainMenu& mainMenu) {
    backButton->GetSignal(Widget::OnLeftClick).Connect([&mainMenu] () {
        mainMenu.back();
    });
}

Options::Options(MainMenu& mainMenu, Settings& settings, const std::string& name) :
        State(mainMenu.getDesktop(), name),
        settings  (settings),
        windowBox (Box::Create(Box::Orientation::VERTICAL)),
        backButton(Button::Create()) {
    auto window          = Window::Create(Window::Style::BACKGROUND);
    auto windowAlignment = Alignment::Create();

    windowAlignment->SetScale({1.0f, 0.0f});
    windowAlignment->SetAlignment({0.0f, 0.0f});
    windowAlignment->Add(windowBox);

    window->Add(windowAlignment);
    window->SetRequisition({512.0f, 300.0f});

    box->SetOrientation(Box::Orientation::VERTICAL);
    box->SetRequisition({300.0f, box->GetRequisition().y});
    box->SetSpacing(20.0f);

    box->SetOrientation(Box::Orientation::VERTICAL);
    box->SetSpacing(20.0f);
    box->Pack(window);
    box->Pack(backButton);

    desktop.Add(box);

    initSignals(mainMenu);
}

void
Options::resetText() {
    backButton->SetLabel(pgtx("options", "Back"));

    onResetText();
}

void
Options::onResetText() {

}

Options::~Options() = default;

Options::Option::Option(const sf::String& label, Widget::Ptr control) :
        label(Label::Create(label)),
        control(control),
        widget(Box::Create()) {
    auto alignment1 = Alignment::Create();
    auto alignment2 = Alignment::Create();

    alignment1->Add(Options::Option::label);

    alignment1->SetScale({0.0f, 0.0f});
    alignment1->SetAlignment({0.0f, 0.5f});

    alignment2->Add(control);

    alignment2->SetScale({0.0f, 0.0f});
    alignment2->SetAlignment({1.0f, 0.5f});

    std::static_pointer_cast<sfg::Box>(widget)->Pack(alignment1);
    std::static_pointer_cast<sfg::Box>(widget)->Pack(alignment2);
    Option::widget->SetClass("options");
}

Options::Option::Option(Widget::Ptr widget) : Options::Option(L"", widget) {}

void
Options::Option::changeText(const sf::String& text) {
    label->SetText(text);
}

Widget::Ptr
Options::Option::getControl() const {
    return control;
}

Widget::Ptr
Options::Option::toWidget() const {
    return widget;
}

}
}
}
