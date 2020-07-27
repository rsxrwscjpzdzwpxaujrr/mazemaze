/*
 * Copyright (c) 2019-2020, Мира Странная <rsxrwscjpzdzwpxaujrr@yahoo.com>
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

#include "Gui.hpp"

namespace mazemaze {

class Game;
class StarSky;
class Settings;
class Saver;

namespace gui {

class State;

namespace states {

class OptionsMenu;

}

class MainMenu : public Gui {
public:
    explicit MainMenu(Settings& settings);
    ~MainMenu() override;

    void onEvent(const sf::Event& event) override;

    void newGame(int mazeWidth, int mazeHeight);
    void resumeGame();
    void stopGame();
    bool isGameOpen();
    void showFps(bool show);

    int  getOptionsState() const;
    void setOptionsState(states::OptionsMenu& options, int state);
    bool getShowFps() const;

private:
    Game* game;
    Saver* saver;
    Background* starSkyBackground;
    Settings& settings;

    int mainState;
    int fpsState;
    int optionsState;

    bool fpsShow;

    void setupGame();
};

}
}
