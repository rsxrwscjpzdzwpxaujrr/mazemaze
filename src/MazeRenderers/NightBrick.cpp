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

#include "NightBrick.hpp"

#include <SFML/OpenGL.hpp>

#include "../Logger.hpp"
#include "../utils.hpp"
#include "../Game.hpp"
#include "../Camera.hpp"

namespace mazemaze {
namespace renderers {

NightBrick::NightBrick(mazemaze::Game& game) :
    Brick(game),
    starSky(1024, 0.0f, Rotation(1.5f, 0.7f, 0.0f)) {}

void
NightBrick::on_tick(float) {
    starSky.set_time(game.get_time());
}

void
NightBrick::set_states() {
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);

    float light0_diffuse[] = {0.35f, 0.4f, 0.45f};
    float light0_ambient[] = {0.05f, 0.055f, 0.06f};
    float light0_position[] = {0.5f, 0.75f, 0.25f, 0.0f};

    glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
    glLightfv(GL_LIGHT0, GL_POSITION, light0_position);

    float light1_diffuse[] = {0.3f, 0.2f, 0.175f};
    float light1_ambient[] = {0.0f, 0.0f, 0.0f};

    glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
    glLightfv(GL_LIGHT1, GL_AMBIENT, light1_ambient);

    glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 0.8f);
    glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 1.0f);

    float fogColor[4] = {0.0f, 0.0f, 0.0f, 1.0f};
    float g_FogDensity = 0.25f;

    glFogfv(GL_FOG_COLOR, fogColor);
    glFogi(GL_FOG_MODE, GL_EXP);
    glFogf(GL_FOG_DENSITY, g_FogDensity);
    glFogf(GL_FOG_START, 0);
    glFogf(GL_FOG_END, 10.0f);
}

void
NightBrick::render_chunks(int chunks[]) {
    auto& position = game.get_player().get_camera().position();

    float light1_position[] = { position.x, position.y, position.z, 1.0f };

    glLightfv(GL_LIGHT1, GL_POSITION, light1_position);

    glEnable(GL_FOG);
    glEnable(GL_LIGHTING);

    MazeRenderer::render_chunks(chunks);

    glDisable(GL_LIGHTING);
    glDisable(GL_FOG);
}

void
NightBrick::render_sky() {
    starSky.render();
}

}
}
