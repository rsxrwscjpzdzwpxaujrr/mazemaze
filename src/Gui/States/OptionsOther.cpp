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

#include "OptionsOther.hpp"

#include "../MainMenu.hpp"

#include "../../utils.hpp"
#include "../../Settings.hpp"

using namespace sfg;

namespace mazemaze {
namespace gui {
namespace states {

void
OptionsOther::initSignals(MainMenu& mainMenu) {
    langCombo->GetSignal(ComboBox::OnSelect).Connect([this, &mainMenu] () {
        auto langs = settings.getSupportedLangs();

        settings.setLang(langs[langCombo->GetSelectedItem()].code);

        mainMenu.reopen();
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
    std::string lang = settings.getLang();
    auto langs = settings.getSupportedLangs();

    for (int i = 0; i < settings.getSupportedLangsCount(); i++) {
        langCombo->AppendItem(langs[i].name);

        if (langs[i].code == lang)
            langCombo->SelectItem(i);
    }

    autosaveCheck->SetActive(settings.getAutosave());
    showFpsCheck->SetActive(settings.getShowFps());
}

OptionsOther::OptionsOther(MainMenu& mainMenu, Settings& settings) : Options(mainMenu, settings),
        langCombo    (ComboBox::Create()),
        autosaveCheck(CheckButton::Create(L"")),
        showFpsCheck (CheckButton::Create(L"")) {
    windowBox->Pack(makeOption(pgtx("options", "Language"), langCombo));
    windowBox->Pack(makeOption(pgtx("options", "Autosave"), autosaveCheck));
    windowBox->Pack(makeOption(pgtx("options", "Show FPS"), showFpsCheck));

    initSignals(mainMenu);
    initOptions();

    center();
}

OptionsOther::~OptionsOther() = default;

}
}
}
