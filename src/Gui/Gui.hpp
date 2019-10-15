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

#include "../IRenderable.hpp"
#include "../ITickable.hpp"

#include "Background.hpp"

namespace mazemaze {
namespace gui {

class State;

class Gui : public ITickable, public IRenderable {
public:
    Gui();
    ~Gui() override;

    void handleEvent(const sf::Event& event);
    virtual void onEvent(const sf::Event& event);

    void tick(float deltaTime) override;
    void render() override;

    void back();
    void backTo(int destState);

    int  addState(State* state);
    void removeState(int state);
    void removeStates();

    void setState(int state, bool back = false);
    void setBackground(Background* background);

    void exit();

    sfg::Desktop& getDesktop();
    int           getState() const;
    State&        getState(int state);
    Background&   getBackground() const;
    bool          isWantExit() const;

private:
    sfg::Desktop desktop;

    Background* background;

    std::vector<State*> states;
    std::stack<int> stateStack;
    int state;

    bool wantExit;
};

}
}
