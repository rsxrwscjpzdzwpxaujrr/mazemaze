/*
 * Copyright (c) 2018-2019, Мира Странная <miraityan2004@gmail.com>
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

#include <cmath>
#include <gettext.h>

#include <SFGUI/SFGUI.hpp>

#include "GraphicEngine.hpp"
#include "Settings.hpp"

#include "Gui/MainMenu.hpp"

using namespace mazemaze;

int
main() {
    setlocale(LC_ALL, "");
    bindtextdomain("mazemaze", "./locale");
    textdomain("mazemaze");
    bind_textdomain_codeset("mazemaze", "UTF-8");

    const int startWindowWidth = 854;
    const int startWindowHeight = 480;

    Settings settings;
    sfg::SFGUI sfgui;
    GraphicEngine& engine = GraphicEngine::getInstance();

    engine.openWindow(startWindowWidth, startWindowHeight, false);

    gui::MainMenu mainMenu(settings);

    engine.loop(sfgui, mainMenu);

    return 0;
}
