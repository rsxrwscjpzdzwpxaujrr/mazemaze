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

#include "OptionsOther.hpp"

#include "../../utils.hpp"
#include "../../Settings.hpp"

using namespace sfg;

namespace mazemaze {
namespace gui {
namespace states {

void
OptionsOther::initSignals() {
    langCombo->GetSignal(ComboBox::OnSelect).Connect([this] () {
        auto langs = settings.getSupportedLangs();

        settings.setLang(langs[langCombo->GetSelectedItem()].code);
    });

    autosaveCheck->GetSignal(Widget::OnLeftClick).Connect([this] () {
        settings.setAutosave(autosaveCheck->IsActive());
    });

    showFpsCheck->GetSignal(Widget::OnLeftClick).Connect([this] () {
        settings.setShowFps(showFpsCheck->IsActive());
    });
}

void
OptionsOther::initOptions() {
    std::string curLang = settings.getLang();
    auto langs = settings.getSupportedLangs();

    int i = 0;

    for (auto lang = langs.begin(); lang < langs.end(); lang++, i++) {
        langCombo->AppendItem(lang->name);

        if (lang->code == curLang)
            langCombo->SelectItem(i);
    }

    autosaveCheck->SetActive(settings.getAutosave());
    showFpsCheck->SetActive(settings.getShowFps());
}

OptionsOther::OptionsOther(MainMenu& mainMenu, Settings& settings) :
        Options(mainMenu, settings, "OptionsOther"),
        langCombo    (ComboBox::Create()),
        autosaveCheck(CheckButton::Create(L"")),
        showFpsCheck (CheckButton::Create(L"")),
        langOpt(Option(langCombo)),
        autosaveOpt(Option(autosaveCheck)),
        showFpsOpt(Option(showFpsCheck)) {
    resetText();

    windowBox->Pack(langOpt.toWidget());
    windowBox->Pack(autosaveOpt.toWidget());
    windowBox->Pack(showFpsOpt.toWidget());

    initSignals();
    initOptions();

    center();
}

void
OptionsOther::onResetText() {
    langOpt.changeText    (pgtx("options", "Language"));
    autosaveOpt.changeText(pgtx("options", "Autosave"));
    showFpsOpt.changeText (pgtx("options", "Show FPS"));
}

OptionsOther::~OptionsOther() = default;

}
}
}
