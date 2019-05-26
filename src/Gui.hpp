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
#include <vector>

#include <SFGUI/Desktop.hpp>

#include "IRenderable.hpp"
#include "ITickable.hpp"

namespace mazemaze {

class Game;
class GuiState;

class Gui : public ITickable, public IRenderable {
public:
    Gui();
    ~Gui() override;

    void handleEvent(sf::Event event);

    void tick(float deltaTime) override;
    void render() override;

    void back();
    void backTo(int destState);

    int  addState(GuiState* state);
    void removeState(int state);

    void setState(int state, bool back = false);
    void setBackground(ITickable* tickable, IRenderable* renderable);

    void exit();

    sfg::Desktop* getDesktop();
    int           getState() const;
    GuiState*     getState(int state);
    IRenderable*  getBackgroundRenderable() const;
    ITickable*    getBackgroundTickable() const;
    bool          isWantExit() const;

private:
    sfg::Desktop desktop;

    IRenderable* backgroundRenderable = nullptr;
    ITickable* backgroundTickable = nullptr;

    std::vector<GuiState*> states;
    std::stack<int> stateStack;
    int state;

    bool wantExit = false;
};

}
