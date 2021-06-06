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

#include "../../utils.hpp"
#include "../../GraphicEngine.hpp"
#include "../../Settings.hpp"

using namespace sfg;

namespace mazemaze {
namespace gui {
namespace states {

OptionsControls::KeyChangeWindow::KeyChangeWindow(OptionsControls& optCtrls) :
        optCtrls(optCtrls),
        window         (Window::Create(Window::Style::BACKGROUND)),
        label          (Label::Create()),
        cancelButton   (Button::Create()),
        okButton       (Button::Create()),
        buttonSeparator(Separator::Create(Separator::Orientation::HORIZONTAL)),
        button(-1),
        opened(false) {
    resetText();

    auto mainBox      = Box::Create(Box::Orientation::VERTICAL);
    auto buttonsBox   = Box::Create(Box::Orientation::HORIZONTAL);

    auto separator   = Separator::Create(Separator::Orientation::VERTICAL);

    buttonSeparator->SetRequisition({0.0f, 0.0f});
    separator->SetRequisition({0.0f, 10.0f});

    buttonsBox->Pack(cancelButton);
    buttonsBox->Pack(buttonSeparator);
    buttonsBox->Pack(okButton);

    mainBox->Pack(separator);
    mainBox->Pack(label);
    mainBox->Pack(buttonsBox);

    mainBox->SetClass("nogap");

    cancelButton->SetClass("small");
    okButton->SetClass("small");
    window->SetClass("light");

    window->Add(mainBox);

    window->SetRequisition({300, 150});

    optCtrls.desktop.Add(window);
    window->Show(opened);
}

OptionsControls::KeyChangeWindow::~KeyChangeWindow() {
    optCtrls.desktop.Remove(window);
}

void
OptionsControls::KeyChangeWindow::tick() {
    if (opened)
        optCtrls.desktop.BringToFront(window);
}

void
OptionsControls::KeyChangeWindow::open(int button) {
    if (opened)
        return;

    KeyChangeWindow::button = button;

    okButton->Show(false);
    buttonSeparator->Show(false);

    opened = true;
    window->Show(opened);

    optCtrls.center();

    for (auto keyButton : optCtrls.keyButtons)
        keyButton->SetState(Widget::State::INSENSITIVE);

    initSignals();
}

bool
OptionsControls::KeyChangeWindow::isOpened() {
    return opened;
}

Window::Ptr
OptionsControls::KeyChangeWindow::getWindow() {
    return window;
}

void
OptionsControls::KeyChangeWindow::resetText() {
    label       ->SetText (pgtx("options", "Press new key"));
    cancelButton->SetLabel(pgtx("options", "Cancel"));
    okButton    ->SetLabel(pgtx("options", "Ok"));
}

void
OptionsControls::KeyChangeWindow::close() {
    GraphicEngine::inst().unwaitKey();

    opened = false;
    window->Show(opened);

    for (auto keyButton : optCtrls.keyButtons)
        keyButton->SetState(Widget::State::NORMAL);
}

void
OptionsControls::KeyChangeWindow::initSignals() {
    GraphicEngine::inst().waitKey([this] (const sf::Keyboard::Key key) {
        okButton->Show(true);
        buttonSeparator->Show(true);

        optCtrls.selectedKey = key;

        sf::String newText = pgtx("options", "Pressed key is %s");
        newText.replace("%s", optCtrls.getKeyName(key));

        label->SetText(newText);

        optCtrls.center();
    });

    cancelButton->GetSignal(Widget::OnLeftClick).Connect([this] () {
        close();
    });

    okButton->GetSignal(Widget::OnLeftClick).Connect([this] () {
        std::string control = optCtrls.keyControls.at(button);

        optCtrls.settings.setKey(control, optCtrls.selectedKey);
        optCtrls.updateKeyButtonLabel(button);
        close();
    });
}

}
}
}
