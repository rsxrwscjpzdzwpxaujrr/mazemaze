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
        State(main_menu.desktop(), "Debug"),
        log_box(Box::Create(Box::Orientation::VERTICAL)),
        window(Window::Create(
            Window::Style::BACKGROUND |
            Window::Style::TITLEBAR |
            Window::Style::RESIZE |
            Window::Style::CLOSE
        )),
        scrolled_window(ScrolledWindow::Create()),
        not_displayed_messages(std::deque<Logger::Message>(
                Logger::inst().messages().begin(),
                Logger::inst().messages().end()
        )),
        showing(false),
        odd(false) {
    Logger::inst().add_message_listener([this] (Logger::Message& message) {
        not_displayed_messages.emplace(message);
    });

    char scrollbar_policy = ScrolledWindow::HORIZONTAL_NEVER | ScrolledWindow::VERTICAL_ALWAYS;

    window->SetClass("log_window");
    window->Add(scrolled_window);
    window->SetTitle(pgtx("debug", "Debug log"));
    window->SetPosition({ 16.0f, 16.0f });

    window->GetSignal(Window::OnCloseButton).Connect([&main_menu] () {
        main_menu.show_debug(false);
    });

    scrolled_window->SetScrollbarPolicy(scrollbar_policy);
    scrolled_window->SetRequisition({ 0.0f, 320.0f });
    scrolled_window->AddWithViewport(log_box);

    log_box->SetRequisition({ 640.0f, 0.0f });
    log_box->SetClass("log_box");

    tick(nullptr, 0.0f);

    desktop.Add(window);
}

Debug::~Debug() = default;


void
Debug::center() {
}

void
Debug::tick(void*, float) {
    if (!showing)
        return;

    bool first = true;
    bool adjustement_pinned = false;

    while (not_displayed_messages.size() > 0) {
        if (first) {
            adjustement_pinned = adjustement_value() == adjustement_upper_value();
            first = false;
        }

        log_box->Pack(create_log_element(not_displayed_messages.front(), odd));
        not_displayed_messages.pop();

        odd = !odd;
    }

    if (adjustement_pinned) {
        scrolled_window->Refresh();
        set_adjustement_value(adjustement_upper_value());
    }
}

void
Debug::show(bool show) {
    State::show(show);

    showing = show;

    if (show)
        desktop.BringToFront(window);
}

Container::Ptr
Debug::main_container() {
    return window;
}

void
Debug::reset_text() {
}

Widget::Ptr
Debug::create_log_element(Logger::Message& message, bool odd) {
    auto element = Window::Create(Window::Style::NO_STYLE);
    element->SetClass("log_element");

    if (odd)
        element->SetStyle(Window::Style::BACKGROUND);

    auto label = Label::Create(message.to_string());

    std::string class_name;

    switch (message.level) {
    case Logger::DEBUG:
        class_name = "debug";
        break;

    case Logger::STATUS:
        class_name = "status";
        break;

    case Logger::WARN:
        class_name = "warn";
        break;

    case Logger::ERR:
        class_name = "error";
        break;
    }

    label->SetClass(class_name);
    label->SetAlignment({ 0.0f, 0.5f });

    element->Add(label);

    return element;
}

float
Debug::adjustement_upper_value() {
    auto adjustement = scrolled_window->GetVerticalAdjustment();

    return adjustement->GetUpper() - adjustement->GetPageSize();
}

float
Debug::adjustement_value() {
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
