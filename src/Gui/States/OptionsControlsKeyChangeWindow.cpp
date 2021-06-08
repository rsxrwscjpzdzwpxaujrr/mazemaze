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

OptionsControls::KeyChangeWindow::KeyChangeWindow(OptionsControls& opt_ctrls) :
        opt_ctrls(opt_ctrls),
        window          (Window::Create(Window::Style::BACKGROUND)),
        label           (Label::Create()),
        cancel_button   (Button::Create()),
        ok_button       (Button::Create()),
        button_separator(Separator::Create(Separator::Orientation::HORIZONTAL)),
        button(-1),
        opened(false) {
    reset_text();

    auto main_box    = Box::Create(Box::Orientation::VERTICAL);
    auto buttons_box = Box::Create(Box::Orientation::HORIZONTAL);

    auto separator   = Separator::Create(Separator::Orientation::VERTICAL);

    button_separator->SetRequisition({0.0f, 0.0f});
    separator->SetRequisition({0.0f, 10.0f});

    buttons_box->Pack(cancel_button);
    buttons_box->Pack(button_separator);
    buttons_box->Pack(ok_button);

    main_box->Pack(separator);
    main_box->Pack(label);
    main_box->Pack(buttons_box);

    main_box->SetClass("nogap");

    cancel_button->SetClass("small");
    ok_button->SetClass("small");
    window->SetClass("light");

    window->Add(main_box);

    window->SetRequisition({300, 150});

    opt_ctrls.desktop.Add(window);
    window->Show(opened);
}

OptionsControls::KeyChangeWindow::~KeyChangeWindow() {
    opt_ctrls.desktop.Remove(window);
}

void
OptionsControls::KeyChangeWindow::tick() {
    if (opened)
        opt_ctrls.desktop.BringToFront(window);
}

void
OptionsControls::KeyChangeWindow::open(int button) {
    if (opened)
        return;

    KeyChangeWindow::button = button;

    ok_button->Show(false);
    button_separator->Show(false);

    opened = true;
    window->Show(opened);

    opt_ctrls.center();

    for (auto key_button : opt_ctrls.key_buttons)
        key_button->SetState(Widget::State::INSENSITIVE);

    init_signals();
}

bool
OptionsControls::KeyChangeWindow::is_opened() {
    return opened;
}

Window::Ptr
OptionsControls::KeyChangeWindow::get_window() {
    return window;
}

void
OptionsControls::KeyChangeWindow::reset_text() {
    label        ->SetText (pgtx("options", "Press new key"));
    cancel_button->SetLabel(pgtx("options", "Cancel"));
    ok_button    ->SetLabel(pgtx("options", "Ok"));
}

void
OptionsControls::KeyChangeWindow::close() {
    GraphicEngine::inst().unwait_key();

    opened = false;
    window->Show(opened);

    for (auto keyButton : opt_ctrls.key_buttons)
        keyButton->SetState(Widget::State::NORMAL);
}

void
OptionsControls::KeyChangeWindow::init_signals() {
    GraphicEngine::inst().wait_key([this] (const sf::Keyboard::Key key) {
        ok_button->Show(true);
        button_separator->Show(true);

        opt_ctrls.selected_key = key;

        sf::String new_text = pgtx("options", "Pressed key is %s");
        new_text.replace("%s", opt_ctrls.get_key_name(key));

        label->SetText(new_text);

        opt_ctrls.center();
    });

    cancel_button->GetSignal(Widget::OnLeftClick).Connect([this] () {
        close();
    });

    ok_button->GetSignal(Widget::OnLeftClick).Connect([this] () {
        std::string control = opt_ctrls.key_controls.at(button);

        opt_ctrls.settings.set_key(control, opt_ctrls.selected_key);
        opt_ctrls.update_key_button_label(button);
        close();
    });
}

}
}
}
