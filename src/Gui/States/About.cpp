/*
 * Copyright (c) 2020-2021, Мира Странная <rsxrwscjpzdzwpxaujrr@yahoo.com>
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

#include "About.hpp"

#include <gettext.h>

#include "../../utils.hpp"

#include "../MainMenu.hpp"

using namespace sfg;

namespace mazemaze {
namespace gui {
namespace states {

About::About(MainMenu& main_menu, Settings& settings) :
        Options(main_menu, settings, "About"),
        about_label(Label::Create()) {
    reset_text();

    about_label->SetLineWrap(true);
    window_box->Pack(about_label);

    center();
}

void
About::on_reset_text() {
    about_label->SetText(pgtxf("about",
            "Mazemaze 0.3-git (under development)\n"
            "Built on %s\n"
            "\n"
            "Simple maze game created by sad girl\n"
            "Mira Strannaya. Licensed under GPL v2.\n"
            "\n"
            "You can read more about me and this game on "
            "my personal website https://mira-strannaya.ru/",
            __DATE__));
}

About::~About() = default;

}
}
}
