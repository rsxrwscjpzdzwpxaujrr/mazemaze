/*
 * Copyright (c) 2021, Мира Странная <rsxrwscjpzdzwpxaujrr@yahoo.com>
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

#include "Debug.hpp"

#include <deque>

#include <SFGUI/Widgets.hpp>

#include "../MainMenu.hpp"

#include "../../Settings.hpp"
#include "../../utils.hpp"

using namespace sfg;

namespace mazemaze {
namespace gui {
namespace states {

Debug::Debug(MainMenu& main_menu) :
        State(main_menu.getDesktop(), "Debug"),
        log_box(Box::Create(Box::Orientation::VERTICAL)),
        window(Window::Create(
            Window::Style::BACKGROUND |
            Window::Style::TITLEBAR |
            Window::Style::RESIZE |
            Window::Style::CLOSE
        )),
        scrolled_window(ScrolledWindow::Create()),
        showing(false),
        odd(false) {
    char scrollbar_policy = ScrolledWindow::HORIZONTAL_NEVER | ScrolledWindow::VERTICAL_ALWAYS;

    scrolled_window->SetScrollbarPolicy(scrollbar_policy);
    scrolled_window->SetRequisition({ 0, 320 });

    window->SetClass("log_window");
    window->Add(scrolled_window);
    window->SetTitle(pgtx("debug", "Debug log"));

    window->GetSignal(Window::OnCloseButton).Connect([&main_menu] () {
        main_menu.showDebug(false);
    });

    scrolled_window->AddWithViewport(log_box);

    log_box->SetRequisition({ 640, 0 });
    log_box->SetClass("log_box");

    tick(0.0f);

    desktop.Add(window);
    desktop.Add(box);
}

Debug::~Debug() = default;

void
Debug::tick(float) {
    if (!showing)
        return;

    bool first = true;
    bool adjustement_pinned = false;

    std::deque<Logger::Message>& messages = Logger::inst().getMessages();

    for (auto& message: messages) {
        if (message.time > last_message) {
            if (first) {
                adjustement_pinned = get_adjustement_value() == get_adjustement_upper_value();
                first = false;
            }

            log_box->Pack(create_log_element(message, odd));

            odd = !odd;
        }
    }

    last_message = messages.back().time;

    if (adjustement_pinned)
        set_adjustement_value(get_adjustement_upper_value());
}

void
Debug::show(bool show) {
    State::show(show);

    showing = show;

    if (show)
        desktop.BringToFront(window);
}

Container::Ptr
Debug::getMainContainer() {
    return window;
}

void
Debug::resetText() {
}

Widget::Ptr
Debug::create_log_element(Logger::Message message, bool odd) {
    auto element = Window::Create(Window::Style::BACKGROUND);

    if (odd)
        element->SetClass("odd_log_element");
    else
        element->SetClass("log_element");

    auto label = Label::Create(message.to_string());

    label->SetClass("log_element");
    label->SetAlignment({ 0.0f, 0.5f });

    element->Add(label);

    return element;
}

float
Debug::get_adjustement_upper_value() {
    auto adjustement = scrolled_window->GetVerticalAdjustment();

    return adjustement->GetUpper() - adjustement->GetPageSize();
}

float
Debug::get_adjustement_value() {
    auto adjustement = scrolled_window->GetVerticalAdjustment();

    return adjustement->GetValue();
}

void
Debug::set_adjustement_value(float value) {
    auto adjustement = scrolled_window->GetVerticalAdjustment();

    adjustement->SetValue(value);
}

}
}
}
