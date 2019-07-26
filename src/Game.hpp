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

#pragma once

#include "Gui/Background.hpp"

#include "Maze.hpp"
#include "MazeRenderer.hpp"
#include "Player.hpp"
#include "Saver.hpp"
#include "IRenderable.hpp"
#include "ITickable.hpp"

namespace mazemaze {
namespace gui {

class MainMenu;

}

class Camera;
class Settings;

class Game : public gui::Background {
public:
    explicit Game(gui::MainMenu& mainMenu, Settings& settings, int mazeWidth, int mazeHeight);
    ~Game() override;

    void newGame();
    void onLoad();

    void render() override;
    void tick(float deltaTime) override;

    void setPaused(bool paused);
    void setWon(bool won);
    void setTime(float time);
    void setWantExit();
    void setRenderer(int id);

    bool isPaused() const;
    bool isWon() const;
    bool isWantExit() const;

    float getTime() const;
    Maze& getMaze();
    Player& getPlayer();
    Camera* getCamera() override;
    MazeRenderer& getRenderer();

private:
    Maze maze;
    int mazeRenderer;
    MazeRenderer* mazeRenderers[16];
    Player player;
    Settings& settings;

    gui::MainMenu& mainMenu;

    float lastSaveTime;

    bool paused;
    bool won;
    bool oldPauseKeyState;
    float time;
    bool wantExit;
};

}
