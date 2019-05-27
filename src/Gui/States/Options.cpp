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

#include "Options.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

#include <SFGUI/Widgets.hpp>

#include "../../GraphicEngine.hpp"

#include "../MainMenu.hpp"

using namespace sfg;

namespace mazemaze {
namespace gui {
namespace states {

inline sf::Vector2f
getSeparatorRequiredRequisition(float requiredWidth, Box::Ptr current) {
    float fullscreenBoxWidth = current->GetRequisition().x;
    float separatorWidth = requiredWidth - fullscreenBoxWidth;

    return {separatorWidth, 0.0f};
}

inline Box::Ptr
addToOptionsList(const sf::String& label, Widget::Ptr widget) {
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

inline void
initSignals(CheckButton::Ptr fullscreenCheck, ComboBox::Ptr antialiasingCombo,
            CheckButton::Ptr vsyncCheck, Button::Ptr backButton, MainMenu* mainMenu) {
    fullscreenCheck->GetSignal(Widget::OnLeftClick).Connect([fullscreenCheck] () {
        GraphicEngine::getInstance()->setFullscreen(fullscreenCheck->IsActive());
    });

    antialiasingCombo->GetSignal(ComboBox::OnSelect).Connect([antialiasingCombo] () {
        int item = antialiasingCombo->GetSelectedItem();
        unsigned int antialiasing = 1;

        for (int i = 0; i < item; i++)
            antialiasing *= 2;

        if (antialiasing == 1)
            antialiasing = 0;

        GraphicEngine::getInstance()->setAntialiasing(antialiasing);
    });

    vsyncCheck->GetSignal(Widget::OnLeftClick).Connect([vsyncCheck] () {
        GraphicEngine::getInstance()->setVsync(vsyncCheck->IsActive());
    });

    backButton->GetSignal(Widget::OnLeftClick).Connect([mainMenu] () {
        mainMenu->back();
    });
}

inline void
initAntialiasingCombo(ComboBox::Ptr antialiasingCombo) {
    int maxAntialiasing;
    glGetIntegerv(GL_SAMPLES_ARB, &maxAntialiasing);

    antialiasingCombo->AppendItem(L"No");

    if (maxAntialiasing >= 2)
        antialiasingCombo->AppendItem(L"2x");

    if (maxAntialiasing >= 4)
        antialiasingCombo->AppendItem(L"4x");

    antialiasingCombo->SelectItem(0);
}

Options::Options(Desktop* desktop, MainMenu* mainMenu) :
                      State(desktop) {
    auto window             = Window::Create(Window::Style::BACKGROUND);
    auto scroll             = ScrolledWindow::Create(Adjustment::Create(), Adjustment::Create());
    auto button             = Button::Create(L"Back");
    auto windowBox          = Box::Create(Box::Orientation::VERTICAL);
    auto fullscreenCheck    = CheckButton::Create(L"");
    auto vsyncCheck         = CheckButton::Create(L"");
    auto antialiasingCombo  = ComboBox::Create();
    auto graphicsGroupLabel = Label::Create(L"Graphics");
    auto separator          = Separator::Create();

    scroll->SetScrollbarPolicy(ScrolledWindow::ScrollbarPolicy::HORIZONTAL_AUTOMATIC |
                               ScrolledWindow::ScrollbarPolicy::VERTICAL_ALWAYS);

    graphicsGroupLabel->SetClass("optionsGroup");

    windowBox->Pack(graphicsGroupLabel);
    windowBox->Pack(addToOptionsList(L"Fullscreen", fullscreenCheck));
    windowBox->Pack(addToOptionsList(L"Antialiasing", antialiasingCombo));
    windowBox->Pack(addToOptionsList(L"V-Sync", vsyncCheck));

    initAntialiasingCombo(antialiasingCombo);

    separator->SetRequisition({440.0f, 0.0f});
    windowBox->Pack(separator);

    scroll->AddWithViewport(windowBox);
    window->Add(scroll);
    window->SetRequisition({512.0f, 300.0f});

    box = Box::Create(Box::Orientation::VERTICAL);
    box->SetSpacing(20.0f);
    box->Pack(window);
    box->Pack(button);

    desktop->Add(box);

    initSignals(fullscreenCheck, antialiasingCombo, vsyncCheck, button, mainMenu);

    center();
}

Options::~Options() = default;

}
}
}
