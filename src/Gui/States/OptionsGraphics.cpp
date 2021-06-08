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

#include "OptionsGraphics.hpp"

#include "../MainMenu.hpp"

#include "../../utils.hpp"
#include "../../Settings.hpp"

#include <memory>

using namespace sfg;

namespace mazemaze {
namespace gui {
namespace states {

void
OptionsGraphics::init_signals() {
    fullscreen_check->GetSignal(Widget::OnLeftClick).Connect([this] () {
        settings.set_fullscreen(fullscreen_check->IsActive());
    });

    antialiasing_combo->GetSignal(ComboBox::OnSelect).Connect([this] () {
        int item = antialiasing_combo->GetSelectedItem();
        unsigned int antialiasing = 1;

        for (int i = 0; i < item; i++)
            antialiasing *= 2;

        if (antialiasing == 1)
            antialiasing = 0;

        settings.set_antialiasing(antialiasing);
    });

    vsync_check->GetSignal(Widget::OnLeftClick).Connect([this] () {
        settings.set_vsync(vsync_check->IsActive());
    });

    style_combo->GetSignal(ComboBox::OnSelect).Connect([this] () {
        settings.set_renderer(style_combo->GetSelectedItem());
    });

    camera_bobbing_check->GetSignal(Widget::OnLeftClick).Connect([this] () {
        settings.set_camera_bobbing(camera_bobbing_check->IsActive());
    });
}

void
OptionsGraphics::init_antialiasing_combo() {
    int max_antialiasing = settings.get_max_antialiasing();

    antialiasing_combo->AppendItem("");

    for (int i = 2; i <= max_antialiasing; i *= 2)
        antialiasing_combo->AppendItem(fmt("%dx", i));
}

void
OptionsGraphics::init_options() {
    fullscreen_check->SetActive(settings.get_fullscreen());

    init_antialiasing_combo();

    unsigned int antialiasing = settings.get_antialiasing();

    if (antialiasing == 0)
        antialiasing = 1;

    for (unsigned int i = 0, j = 1; j <= antialiasing; i++, j *= 2)
        if (j == antialiasing) {
            antialiasing_combo->SelectItem(i);
            break;
        }

    vsync_check->SetActive(settings.get_vsync());

    style_combo->AppendItem("");
    style_combo->AppendItem("");
    style_combo->AppendItem("");
    style_combo->AppendItem("");
    style_combo->SelectItem(settings.get_renderer());

    camera_bobbing_check->SetActive(settings.get_camera_bobbing());
}

OptionsGraphics::OptionsGraphics(MainMenu& main_menu, Settings& settings) :
        Options(main_menu, settings, "OptionsGraphics"),
        fullscreen_check    (CheckButton::Create(L"")),
        vsync_check         (CheckButton::Create(L"")),
        antialiasing_combo  (ComboBox::Create()),
        style_combo         (ComboBox::Create()),
        camera_bobbing_check(CheckButton::Create(L"")),
        fullscreen_opt    (Option("", fullscreen_check)),
        vsync_opt         (Option("", vsync_check)),
        antialiasing_opt  (Option("", antialiasing_combo)),
        style_opt         (Option("", style_combo)),
        camera_bobbing_opt(Option("", camera_bobbing_check)) {
    window_box->Pack(fullscreen_opt.to_widget());
    window_box->Pack(antialiasing_opt.to_widget());
    window_box->Pack(vsync_opt.to_widget());
    window_box->Pack(style_opt.to_widget());
    window_box->Pack(camera_bobbing_opt.to_widget());

    init_signals();
    init_options();

    reset_text();

    center();
}

void
OptionsGraphics::on_reset_text() {
    fullscreen_opt   .change_text(pgtx("options", "Fullscreen"));
    vsync_opt        .change_text(pgtx("options", "V-Sync"));
    antialiasing_opt .change_text(pgtx("options", "Antialiasing"));
    style_opt        .change_text(pgtx("options", "Style"));
    camera_bobbing_opt.change_text(pgtx("options", "Camera Bobbing"));

    style_combo->ChangeItem(0, pgtx("options", "Classic"));
    style_combo->ChangeItem(1, pgtx("options", "Gray"));
    style_combo->ChangeItem(2, pgtx("options", "Brick"));
    style_combo->ChangeItem(3, pgtx("options", "Night Brick"));

    style_combo->RequestResize();

    antialiasing_combo->ChangeItem(0, pgtx("options", "No"));

    antialiasing_combo->RequestResize();
}

OptionsGraphics::~OptionsGraphics() = default;

}
}
}
