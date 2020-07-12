/*
 * Copyright (c) 2020, Мира Странная <rsxrwscjpzdzwpxaujrr@yahoo.com>
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

#include "../MainMenu.hpp"

using namespace sfg;

namespace mazemaze {
namespace gui {
namespace states {

About::About(MainMenu& mainMenu, Settings& settings) :
        Options(mainMenu, settings),
        aboutLabel(Label::Create("Mazemaze 0.3-git (under development)\n"
                                 "2018 - 2020\n"
                                 "\n"
                                 "Simple maze game created by sad girl\n"
                                 "Mira Strannaya. Licensed under GPL v2.\n"
                                 "\n"
                                 "You can read more about me and this game on "
                                 "my personal website https://mira-strannaya.ru/")) {
    aboutLabel->SetLineWrap(true);
    windowBox->Pack(aboutLabel);

    center();
}

About::~About() = default;

}
}
}
