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

#include "OptionsControls.hpp"

#include "../MainMenu.hpp"

#include "../../utils.hpp"
#include "../../Settings.hpp"
#include "../../GraphicEngine.hpp"

using namespace sfg;

namespace mazemaze {
namespace gui {
namespace states {

OptionsControls::OptionsControls(MainMenu& main_menu, Settings& settings) :
        Options(main_menu, settings, "OptionsControls"),
        sensitivity_slider(Scale::Create(Scrollbar::Orientation::HORIZONTAL)),
        sensitivity_adjustement(sensitivity_slider->GetAdjustment()),
        key_change_window(*this),
        sensitivity_opt("", sensitivity_slider),
        key_opts {
            Option(Button::Create()),
            Option(Button::Create()),
            Option(Button::Create()),
            Option(Button::Create())
        },
        key_controls {
            "up",
            "down",
            "right",
            "left",
        } {
    reset_text();

    sf::Vector2f button_size         = {200.0f,  28.0f};
    sf::Vector2f adjustmement_bounds = {0.0001f, 0.003f};

    window_box->Pack(sensitivity_opt.to_widget());

    sensitivity_slider->SetRequisition(button_size);

    sensitivity_adjustement = sensitivity_slider->GetAdjustment();

    sensitivity_adjustement->SetLower(adjustmement_bounds.x);
    sensitivity_adjustement->SetUpper(adjustmement_bounds.y);

    sensitivity_adjustement->SetMinorStep(
        (adjustmement_bounds.y - adjustmement_bounds.x) / button_size.x);

    for (int i = 0; i < buttons_count; i++) {
        key_buttons[i] = std::dynamic_pointer_cast<sfg::Button>(key_opts[i].control());
        update_key_button_label(i);

        key_buttons[i]->SetRequisition(button_size);
        key_buttons[i]->SetClass("verySmall");
        key_buttons[i]->SetZOrder(0);

        window_box->Pack(key_opts[i].to_widget());
    }

    init_signals();

    center();
}

OptionsControls::~OptionsControls() = default;

void
OptionsControls::show(bool show) {
    State::show(show);

    if (show)
        sensitivity_adjustement->SetValue(settings.sensitivity());

    if (!show && key_change_window.is_opened())
        key_change_window.close();
}

void
OptionsControls::tick(void*, float) {
    key_change_window.tick();
}

void
OptionsControls::center() {
    State::center();

    if (key_change_window.is_opened())
        State::center(key_change_window.window());
}

void
OptionsControls::on_reset_text() {
    key_labels = {
        pgtx("options", "Forward"),
        pgtx("options", "Backward"),
        pgtx("options", "Right"),
        pgtx("options", "Left")
    };

    for (int i = 0; i < buttons_count; i++)
        key_opts[i].change_text(key_labels[i]);

    sensitivity_opt.change_text(pgtx("options", "Mouse sensitivity"));

    key_change_window.reset_text();
}

void
OptionsControls::update_key_button_label(int button) {
    std::string control = key_controls.at(button);

    key_buttons.at(button)->SetLabel(key_name(settings.key(control)));
}

void
OptionsControls::init_signals() {
    sensitivity_adjustement->GetSignal(Adjustment::OnChange).Connect([this] () {
        settings.set_sensitivity(sensitivity_adjustement->GetValue());
    });

    for (int i = 0; i < buttons_count; i++) {
        key_buttons.at(i)->GetSignal(Widget::OnLeftClick).Connect([this, i] () {
            key_change_window.open(i);
        });
    }
}

}
}
}
