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
Options::init_signals(MainMenu& main_menu) {
    back_button->GetSignal(Widget::OnLeftClick).Connect([&main_menu] () {
        main_menu.back();
    });
}

Options::Options(MainMenu& main_menu, Settings& settings, const std::string& name) :
        State(main_menu.get_desktop(), name),
        settings  (settings),
        window_box (Box::Create(Box::Orientation::VERTICAL)),
        back_button(Button::Create()) {
    auto window           = Window::Create(Window::Style::BACKGROUND);
    auto window_alignment = Alignment::Create();

    window_alignment->SetScale({1.0f, 0.0f});
    window_alignment->SetAlignment({0.0f, 0.0f});
    window_alignment->Add(window_box);

    window->Add(window_alignment);
    window->SetRequisition({512.0f, 300.0f});

    box->SetOrientation(Box::Orientation::VERTICAL);
    box->SetRequisition({300.0f, box->GetRequisition().y});
    box->SetSpacing(20.0f);

    box->SetOrientation(Box::Orientation::VERTICAL);
    box->SetSpacing(20.0f);
    box->Pack(window);
    box->Pack(back_button);

    desktop.Add(box);

    init_signals(main_menu);
}

void
Options::reset_text() {
    back_button->SetLabel(pgtx("options", "Back"));

    on_reset_text();
}

void
Options::on_reset_text() {

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
Options::Option::change_text(const sf::String& text) {
    label->SetText(text);
}

Widget::Ptr
Options::Option::get_control() const {
    return control;
}

Widget::Ptr
Options::Option::to_widget() const {
    return widget;
}

}
}
}
