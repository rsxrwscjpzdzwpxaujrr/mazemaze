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

#include "Brick.hpp"

#include <SFML/OpenGL.hpp>

#include "../Logger.hpp"
#include "../utils.hpp"
#include "../Chunk.hpp"
#include "../Game.hpp"
#include "../Camera.hpp"
#include "../path_separator.hpp"

#include <OBJ_Loader.h>

namespace mazemaze {
namespace renderers {

Brick::Brick(Game& game) :
        MazeRenderer(game),
        game(game),
        mesh_count(10),
        skybox(50, 0.5f, 0.5f, 0.5f),
        mesh_draw_list(-1) {}

Brick::~Brick() = default;

void
Brick::compile_walls() {
    Logger::inst().log_debug("Compiling walls.");

    objl::Loader loader = objl::Loader();
    loader.LoadFile("data" PATH_SEPARATOR "wall.obj");

    if (mesh_draw_list != -1)
        glDeleteLists(mesh_draw_list, mesh_count);

    mesh_draw_list = glGenLists(mesh_count);

    for (auto& mesh : loader.LoadedMeshes) {
        bool initialized = false;
        bool side = false;
        Angle angle_type;

        if (mesh.MeshName == "flat") {
            angle_type = Angle::NO;
            initialized = true;
        } else if (mesh.MeshName == "inner_left") {
            angle_type = Angle::INNER;
            side = false;
            initialized = true;
        } else if (mesh.MeshName == "inner_right") {
            angle_type = Angle::INNER;
            side = true;
            initialized = true;
        } else if (mesh.MeshName == "outer_left") {
            angle_type = Angle::OUTER;
            side = false;
            initialized = true;
        } else if (mesh.MeshName == "outer_right") {
            angle_type = Angle::OUTER;
            side = true;
            initialized = true;
        } else {
            continue;
        }

        if (initialized) {
            compile_wall(mesh, angle_type, false, side);
            compile_wall(mesh, angle_type, true,  side);
        }
    }
}

void
Brick::compile_wall(objl::Mesh& mesh, Angle angle_type, bool v_mirror, bool side) {
    float y_coeff;
    int j, j_end, j_step;

    if (v_mirror) {
        y_coeff = -1.0f;
        j = mesh.Indices.size() - 1; j_end = -1; j_step = -1;
    } else {
        y_coeff = 1.0f;
        j = 0; j_end = mesh.Indices.size(); j_step = 1;
    }

    glNewList(get_mesh(angle_type, v_mirror, side), GL_COMPILE);

    glBegin(GL_TRIANGLES);
    glColor3f(0.42f, 0.2f, 0.16f);

    for (; j != j_end; j += j_step) {
        objl::Vertex vertex = mesh.Vertices[mesh.Indices[j]];

        glNormal3f(vertex.Normal.X, vertex.Normal.Y * y_coeff, vertex.Normal.Z);

        glVertex3f(vertex.Position.X,
                   vertex.Position.Y * y_coeff,
                   vertex.Position.Z);
    }

    draw_mortar(angle_type, side);

    glEndList();
}

void
Brick::draw_mortar(Brick::Angle angle_type, bool side) {
    float x_start = 0.0f;
    float x_end = 0.0f;
    float x_offset;

    if (side)
        x_offset = 0.5f;
    else
        x_offset = 0.0f;

    float* x_changing;

    if (side)
        x_changing = &x_end;
    else
        x_changing = &x_start;

    switch (angle_type) {
    case Angle::INNER:
        *x_changing = -0.0075f;
        break;

    case Angle::OUTER:
        *x_changing = 0.0075f;
        break;
    }

    x_end *= -1.0f;

    glColor3f(0.375f, 0.375f, 0.375f);

    glNormal3f(0.0f, 0.0f, 1.0f);

    glVertex3f(x_offset + x_end + 0.5f,  0.5f, -0.5075f);
    glVertex3f(x_offset + x_start,       0.5f, -0.5075f);
    glVertex3f(x_offset + x_start,      -0.5f, -0.5075f);

    glNormal3f(0.0f, 0.0f, 1.0f);

    glVertex3f(x_offset + x_end + 0.5f, -0.5f, -0.5075f);
    glVertex3f(x_offset + x_end + 0.5f,  0.5f, -0.5075f);
    glVertex3f(x_offset + x_start,      -0.5f, -0.5075f);

    glEnd();
}

unsigned int
Brick::get_mesh(Brick::Angle angle_type, bool v_mirror, bool side) {
    if (angle_type == Angle::NO)
        return mesh_draw_list + v_mirror * (mesh_count / 2);

    return mesh_draw_list + 1 + (angle_type - 1) * 2 + side + v_mirror * (mesh_count / 2);
}

Brick::Angle
Brick::get_angle(bool openeds[]) {
    if (openeds[1])
        return Angle::OUTER;

    return Angle(!openeds[0]);
}

void
Brick::render_wall(Angle left_angle, Angle right_angle, bool flip) {
    glTranslatef(-0.5f, 0.0f, 0.0f);
    glCallList(get_mesh(left_angle, flip, false));

    if (right_angle == Angle::NO)
        glTranslatef(0.5f, 0.0f, 0.0f);

    glCallList(get_mesh(right_angle, flip, true));
}

void
Brick::set_states() {
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);

    float light0_diffuse[] = {0.5f, 0.45f, 0.4f};
    float light0_ambient[] = {0.33f, 0.4f, 0.5f};
    float light0_position[] = {0.5f, 0.75f, 0.25f, 0.0f};

    glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
    glLightfv(GL_LIGHT0, GL_POSITION, light0_position);

    float light1_diffuse[] = {0.25f, 0.225f, 0.2f};
    float light1_ambient[] = {0.0f, 0.0f, 0.0f};

    glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
    glLightfv(GL_LIGHT1, GL_AMBIENT, light1_ambient);

    glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 0.4f);
    glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.2f);

    float fog_color[4] = {0.5f, 0.5f, 0.5f, 1.0f};
    float fog_density = 0.5f;

    glFogfv(GL_FOG_COLOR, fog_color);
    glFogi(GL_FOG_MODE, GL_EXP);
    glFogf(GL_FOG_DENSITY, fog_density);
    glFogf(GL_FOG_START, 0);
    glFogf(GL_FOG_END, 10.0f);
}

void
Brick::on_enable() {
    compile_walls();
}

void
Brick::on_disable() {
    glDisable(GL_LIGHT0);
    glDisable(GL_LIGHT1);

    if (mesh_draw_list != -1)
        glDeleteLists(mesh_draw_list, mesh_count);
}

void
Brick::compile_chunk(int num) {
    int x = (num % maze.get_chunks_x()) * Chunk::SIZE;
    int y = (num / maze.get_chunks_x()) * Chunk::SIZE;

    Logger::inst().log_debug(fmt("Compiling chunk %d at %d %d.", num, x, y));

    glNewList(draw_list + num, GL_COMPILE);

    glPushMatrix();

    glTranslatef(x, 0.0, y);

    int endX = Chunk::SIZE;
    int endY = Chunk::SIZE;

    if (x + Chunk::SIZE > maze.get_width())
        endX = maze.get_width() % Chunk::SIZE;

    if (y + Chunk::SIZE > maze.get_height())
        endY = maze.get_height() % Chunk::SIZE;

    for (int j = 0; j < endX; j++)
        for (int k = 0; k < endY; k++) {
            glColor3f(0.4f, 0.4f, 0.4f);
            glNormal3f(0.0f, 1.0f, 0.0f);

            glBegin(GL_QUADS);
            glVertex3i(j, 0, k);
            glVertex3i(j, 0, k + 1);
            glVertex3i(j + 1, 0, k + 1);
            glVertex3i(j + 1, 0, k);
            glEnd();

            if (maze.get_opened(j + x, k + y)) {
                glPushMatrix();
                glTranslatef(j + 0.5f, 0.5f, k + 0.5f);

                bool opened[] = {maze.get_opened(j + 1 + x, k + y),
                                 maze.get_opened(j - 1 + x, k + y),
                                 maze.get_opened(j + x, k + 1 + y),
                                 maze.get_opened(j + x, k - 1 + y)};

                if (!opened[0]) {
                    glPushMatrix();

                    glRotatef(270.0f, 0.0f, 1.0f, 0.0f);

                    bool tmpOpened[] = {maze.get_opened(j + 1 + x, k + 1 + y),
                                        maze.get_opened(j + 1 + x, k - 1 + y)};

                    bool angles[][2] = {{opened[3], tmpOpened[1]},
                                        {opened[2], tmpOpened[0]}};

                    render_wall(get_angle(angles[0]), get_angle(angles[1]), true);
                    glPopMatrix();
                }

                if (!opened[1]) {
                    glPushMatrix();

                    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);

                    bool tmpOpened[] = {maze.get_opened(j - 1 + x, k + 1 + y),
                                        maze.get_opened(j - 1 + x, k - 1 + y)};

                    bool angles[][2] = {{opened[2], tmpOpened[0]},
                                        {opened[3], tmpOpened[1]}};

                    render_wall(get_angle(angles[0]), get_angle(angles[1]), true);
                    glPopMatrix();
                }

                if (!opened[2]) {
                    glPushMatrix();
                    glRotatef(180.0f, 0.0f, 1.0f, 0.0f);

                    bool tmpOpened[] = {maze.get_opened(j + 1 + x, k + 1 + y),
                                        maze.get_opened(j - 1 + x, k + 1 + y)};

                    bool angles[][2] = {{opened[0], tmpOpened[0]},
                                        {opened[1], tmpOpened[1]}};

                    render_wall(get_angle(angles[0]), get_angle(angles[1]), false);
                    glPopMatrix();
                }

                if (!opened[3]) {
                    bool tmpOpened[] = {maze.get_opened(j + 1 + x, k - 1 + y),
                                        maze.get_opened(j - 1 + x, k - 1 + y)};

                    bool angles[][2] = {{opened[1], tmpOpened[1]},
                                        {opened[0], tmpOpened[0]}};

                    render_wall(get_angle(angles[0]), get_angle(angles[1]), false);
                }

                glPopMatrix();
            }
        }

    glPopMatrix();

    glEndList();

    compiled[num] = true;
}

void
Brick::on_tick(float) {
}

void
Brick::render_chunks(int chunks[]) {
    Camera& camera = game.get_player().get_camera();

    float light1_position[] = {camera.get_x(), camera.get_y(), camera.get_z(), 1.0f};

    glLightfv(GL_LIGHT1, GL_POSITION, light1_position);

    glEnable(GL_FOG);
    glEnable(GL_LIGHTING);

    MazeRenderer::render_chunks(chunks);

    glDisable(GL_LIGHTING);
    glDisable(GL_FOG);
}

void
Brick::render_sky() {
    skybox.render();
}

}
}
