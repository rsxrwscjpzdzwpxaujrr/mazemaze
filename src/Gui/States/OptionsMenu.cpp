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

#include "OptionsMenu.hpp"

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
OptionsMenu::init_signals(MainMenu& main_menu) {
    back_button->GetSignal(Widget::OnLeftClick).Connect([&main_menu] () {
        main_menu.back();
    });

    graphics_button->GetSignal(Widget::OnLeftClick).Connect([&main_menu, this] () {
        main_menu.set_state(graphics_state);
    });

    controls_button->GetSignal(Widget::OnLeftClick).Connect([&main_menu, this] () {
        main_menu.set_state(controls_state);
    });

    other_button->GetSignal(Widget::OnLeftClick).Connect([&main_menu, this] () {
        main_menu.set_state(other_state);
    });
}

OptionsMenu::OptionsMenu(MainMenu& main_menu, Settings& settings) :
        State(main_menu.desktop(), "OptionsMenu"),
        graphics_button (Button::Create()),
        controls_button (Button::Create()),
        other_button    (Button::Create()),
        back_button     (Button::Create()) {
    reset_text();

    box->SetOrientation(Box::Orientation::VERTICAL);
    box->SetRequisition({300.0f, box->GetRequisition().y});
    box->SetSpacing(20.0f);

    box->Pack(graphics_button);
    box->Pack(controls_button);
    box->Pack(other_button);
    box->Pack(back_button);

    desktop.Add(box);

    init_signals(main_menu);

    center();

    graphics_state = main_menu.add_state(new OptionsGraphics(main_menu, settings));
    controls_state = main_menu.add_state(new OptionsControls(main_menu, settings));
    other_state    = main_menu.add_state(new OptionsOther   (main_menu, settings));
}

void
OptionsMenu::reset_text() {
    graphics_button->SetLabel(pgtx("options", "Graphics"));
    controls_button->SetLabel(pgtx("options", "Controls"));
    other_button   ->SetLabel(pgtx("options", "Other"));
    back_button    ->SetLabel(pgtx("options", "Back"));
}

OptionsMenu::~OptionsMenu() = default;

}
}
}
