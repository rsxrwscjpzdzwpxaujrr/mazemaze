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

Box::Ptr
OptionsOther::addToOptionsList(const sf::String& label, Widget::Ptr widget) {
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
OptionsOther::initSignals(MainMenu& mainMenu) {
    langCombo->GetSignal(ComboBox::OnSelect).Connect([this] () {
        const std::string* const langCodes = settings.getSupportedLangs();

        settings.setLang(langCodes[langCombo->GetSelectedItem()]);
    });

    autosaveCheck->GetSignal(Widget::OnLeftClick).Connect([this] () {
        settings.setAutosave(autosaveCheck->IsActive());
    });

    backButton->GetSignal(Widget::OnLeftClick).Connect([&mainMenu] () {
        mainMenu.back();
    });

    showFpsCheck->GetSignal(Widget::OnLeftClick).Connect([this] () {
        settings.setShowFps(showFpsCheck->IsActive());
    });
}

void
OptionsOther::initOptions() {
    langCombo->AppendItem(L"English");
    langCombo->AppendItem(L"Русский");
    langCombo->AppendItem(L"Українська");
    langCombo->AppendItem(L"Қазақша");

    std::string lang = settings.getLang();
    const std::string* const langCodes = settings.getSupportedLangs();

    for (int i = 0; i < settings.getSupportedLangsCount(); i++)
      if (langCodes[i] == lang)
          langCombo->SelectItem(i);

    autosaveCheck->SetActive(settings.getAutosave());
    showFpsCheck->SetActive(settings.getShowFps());
}

OptionsOther::OptionsOther(sfg::Desktop& desktop, MainMenu& mainMenu, Settings& settings) :
        State(desktop),
        settings(settings),
        backButton   (Button::Create(pgtx("options", "Back"))),
        langCombo    (ComboBox::Create()),
        autosaveCheck(CheckButton::Create(L"")),
        showFpsCheck (CheckButton::Create(L"")) {
    auto window          = Window::Create(Window::Style::BACKGROUND);
    auto windowBox       = Box::Create(Box::Orientation::VERTICAL);
    auto windowAlignment = Alignment::Create();

    windowBox->Pack(addToOptionsList(pgtx("options", "Language"), langCombo));
    windowBox->Pack(addToOptionsList(pgtx("options", "Autosave"), autosaveCheck));
    windowBox->Pack(addToOptionsList(pgtx("options", "Show FPS"), showFpsCheck));

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

OptionsOther::~OptionsOther() = default;

}
}
}
