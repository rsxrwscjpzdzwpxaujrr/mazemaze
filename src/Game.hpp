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

#include "Maze.hpp"
#include "MazeRenderer.hpp"
#include "Player.hpp"
#include "StarSky.hpp"
#include "Saver.hpp"
#include "IRenderable.hpp"
#include "ITickable.hpp"

namespace mazemaze {

class MainMenu;

class Game : public IRenderable, public ITickable {
public:
    explicit Game(MainMenu* mainMenu, int mazeWidth, int mazeHeight);
    ~Game() override;

    void newGame();
    void onLoad();

    void render() override;
    void tick(float deltaTime) override;

    void setPaused(bool paused);
    void setWon(bool won);
    void setTime(float time);
    void setWantExit();

    bool isPaused() const;
    bool isWon() const;
    bool isWantExit() const;

    float getTime() const;
    Maze* getMaze();
    Player* getPlayer();

private:
    Maze maze;
    MazeRenderer mazeRenderer;
    Player player;
    StarSky starSky;

    MainMenu* mainMenu;

    Saver saver;
    float lastSaveTime = 0;
    const float saveInterval = 60.0f;

    bool paused = false;
    bool won = false;
    int oldPauseKeyState = false;
    float time = 0.0f;
    bool wantExit = false;
};

}
