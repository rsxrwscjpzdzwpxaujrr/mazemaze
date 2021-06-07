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

#pragma once

#include "../State.hpp"

#include "../../FpsCalculator.hpp"

namespace mazemaze {

class Settings;

namespace gui {

class MainMenu;

namespace states {

class FpsOverlay : public State {
public:
    explicit FpsOverlay(MainMenu& mainMenu, Settings& settings);
    ~FpsOverlay() override;

    void show(bool show) override;

    void resetText() override;

private:
    sfg::Label::Ptr fpsLabel;
    Settings& settings;
    FpsCalculator fpsCalculator;

    bool showing;

    void tick(void*, float deltaTime) override;
    void center() override;
};

}
}
}
