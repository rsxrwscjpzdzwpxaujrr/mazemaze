/*
 * Copyright (c) 2019, Мира Странная <miraityan2004@gmail.com>
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

#include <stack>

#include <SFGUI/Desktop.hpp>
#include <SFGUI/Box.hpp>

#include "IRenderable.hpp"
#include "ITickable.hpp"

namespace mazemaze {

class MenuState;

class MainMenu {
public:
    MainMenu();
    ~MainMenu() = default;

    void handleEvent(sf::Event event);
    void show(bool show);
    void tick(float deltatime);
    void render();
    void back();
    void backTo(uint destState);

    void setState(uint state, bool back = false);
    uint getState() const;

    void startGame(int mazeWidth, int mazeHeight);
    void stopGame();

    void exit();
    bool isWantExit();

private:
    sfg::Desktop desktop;

    IRenderable* backgroundRenderable = nullptr;
    ITickable* backgroundTickable = nullptr;

    std::vector<MenuState*> states;
    std::stack<uint> stateStack;
    uint state;

    bool wantExit;

    inline void setupStarSky();
};

}
