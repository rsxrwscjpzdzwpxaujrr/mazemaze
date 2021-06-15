/*
 * Copyright (c) 2018-2021, Мира Странная <rsxrwscjpzdzwpxaujrr@yahoo.com>
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
#include "Logger.hpp"
#include "utils.hpp"

#include "Gui/MainMenu.hpp"

using namespace mazemaze;

int
main() {
    {
        using namespace std::chrono;

        auto init_time = Logger::inst().init_time().time_since_epoch();

        auto time_string = fmt(
            "%d.%03d",
            duration_cast<seconds>(init_time).count(),
            duration_cast<milliseconds>(init_time).count() % 1000
        );

        Logger::inst().log_status(fmt("Starting at %s", time_string.c_str()));
    }

    bindtextdomain("mazemaze", "locale");
    textdomain("mazemaze");
    bind_textdomain_codeset("mazemaze", "UTF-8");

    Settings settings;
    sfg::SFGUI sfgui;
    GraphicEngine& engine = GraphicEngine::inst();

    engine.open_window();

    gui::MainMenu main_menu(settings);

    settings.set_main_menu(&main_menu);

    engine.loop(sfgui, main_menu);

    return 0;
}
