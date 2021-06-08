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

#include "OptionsOther.hpp"

#include "../../utils.hpp"
#include "../../Settings.hpp"

using namespace sfg;

namespace mazemaze {
namespace gui {
namespace states {

void
OptionsOther::init_signals() {
    lang_combo->GetSignal(ComboBox::OnSelect).Connect([this] () {
        auto langs = settings.get_supported_langs();

        settings.set_lang(langs[lang_combo->GetSelectedItem()].code);
    });

    autosave_check->GetSignal(Widget::OnLeftClick).Connect([this] () {
        settings.set_autosave(autosave_check->IsActive());
    });

    show_fps_check->GetSignal(Widget::OnLeftClick).Connect([this] () {
        settings.set_show_fps(show_fps_check->IsActive());
    });
}

void
OptionsOther::init_options() {
    std::string cur_lang = settings.get_lang();
    auto langs = settings.get_supported_langs();

    int i = 0;

    for (auto lang = langs.begin(); lang < langs.end(); lang++, i++) {
        lang_combo->AppendItem(lang->name);

        if (lang->code == cur_lang)
            lang_combo->SelectItem(i);
    }

    autosave_check->SetActive(settings.get_autosave());
    show_fps_check->SetActive(settings.get_show_fps());
}

OptionsOther::OptionsOther(MainMenu& main_menu, Settings& settings) :
        Options(main_menu, settings, "OptionsOther"),
        lang_combo    (ComboBox::Create()),
        autosave_check(CheckButton::Create(L"")),
        show_fps_check(CheckButton::Create(L"")),
        lang_opt    (Option(lang_combo)),
        autosave_opt(Option(autosave_check)),
        show_fps_opt(Option(show_fps_check)) {
    reset_text();

    window_box->Pack(lang_opt.to_widget());
    window_box->Pack(autosave_opt.to_widget());
    window_box->Pack(show_fps_opt.to_widget());

    init_signals();
    init_options();

    center();
}

void
OptionsOther::on_reset_text() {
    lang_opt.change_text    (pgtx("options", "Language"));
    autosave_opt.change_text(pgtx("options", "Autosave"));
    show_fps_opt.change_text(pgtx("options", "Show FPS"));
}

OptionsOther::~OptionsOther() = default;

}
}
}
