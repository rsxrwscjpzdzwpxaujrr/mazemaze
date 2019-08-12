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

#include "Brick.hpp"

#include <SFML/OpenGL.hpp>

#include "../Chunk.hpp"
#include "../Game.hpp"
#include "../Camera.hpp"

#include <OBJ_Loader.h>

namespace mazemaze {
namespace renderers {

Brick::Brick(Game& game) :
        MazeRenderer(game),
        game(game),
        skybox(50, 0.5f, 0.5f, 0.5f),
        meshDrawList(-1) {}

Brick::~Brick() = default;

void
Brick::compileMesh() {
    objl::Loader loader = objl::Loader();
    loader.LoadFile("data/wall.obj");

    if (meshDrawList != -1)
        glDeleteLists(meshDrawList, 1);

    meshDrawList = glGenLists(1);

    glNewList(meshDrawList, GL_COMPILE);

    glBegin(GL_TRIANGLES);

    for (int i = 0; i < loader.LoadedMeshes.size(); i++) {
        objl::Mesh mesh = loader.LoadedMeshes[i];

        if (i == 1)
            glColor3f(0.45f, 0.185f, 0.16f);
        else
            glColor3f(0.45f, 0.45f, 0.45f);

        for (int j = 0; j < mesh.Indices.size(); j++) {
            objl::Vertex vertex = mesh.Vertices[mesh.Indices[j]];

            glNormal3f(vertex.Normal.X, vertex.Normal.Y, vertex.Normal.Z);
            glVertex3f(vertex.Position.X, vertex.Position.Y, vertex.Position.Z);
        }
    }

    glEnd();

    glEndList();
}

void
Brick::onEnable() {
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

    float fogColor[4] = {0.5f, 0.5f, 0.5f, 1.0f};
    float g_FogDensity = 0.5f;

    glFogfv(GL_FOG_COLOR, fogColor);
    glFogi(GL_FOG_MODE, GL_EXP);
    glFogf(GL_FOG_DENSITY, g_FogDensity);
    glFogf(GL_FOG_START, 0);
    glFogf(GL_FOG_END, 10.0f);

    compileMesh();
}

void
Brick::onDisable() {
    glDisable(GL_LIGHT0);
    glDisable(GL_LIGHT1);

    if (meshDrawList != -1)
        glDeleteLists(meshDrawList, 1);
}

void
Brick::compileChunk(int num) {
    glNewList(drawList + num, GL_COMPILE);

    glPushMatrix();

    int x = (num % maze.getChunksX()) * Chunk::SIZE;
    int y = (num / maze.getChunksX()) * Chunk::SIZE;

    glTranslatef(x, 0.0, y);

    int endX = Chunk::SIZE;
    int endY = Chunk::SIZE;

    if (x + Chunk::SIZE > maze.getWidth())
        endX = maze.getWidth() % Chunk::SIZE;

    if (y + Chunk::SIZE > maze.getHeight())
        endY = maze.getHeight() % Chunk::SIZE;

    for (int j = 0; j < endX; j++)
        for (int k = 0; k < endY; k++) {
            glColor3f(0.5f, 0.5f, 0.5f);
            glNormal3f(0.0f, 1.0f, 0.0f);

            glBegin(GL_QUADS);
            glVertex3i(j, 0, k);
            glVertex3i(j, 0, k + 1);
            glVertex3i(j + 1, 0, k + 1);
            glVertex3i(j + 1, 0, k);
            glEnd();

            if (maze.getOpened(j + x, k + y)) {
                glPushMatrix();
                glTranslatef(j + 0.5f, 0.0f, k + 0.5f);

                if (!maze.getOpened(j + 1 + x, k + y)) {
                    glPushMatrix();
                    glRotatef(270.0f, 0.0f, 1.0f, 0.0f);

                    glCallList(meshDrawList);
                    glPopMatrix();
                }

                if (!maze.getOpened(j - 1 + x, k + y)) {
                    glPushMatrix();
                    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);

                    glCallList(meshDrawList);
                    glPopMatrix();
                }

                if (!maze.getOpened(j + x, k + 1 + y)) {
                    glPushMatrix();
                    glRotatef(180.0f, 0.0f, 1.0f, 0.0f);

                    glCallList(meshDrawList);
                    glPopMatrix();
                }

                if (!maze.getOpened(j + x, k - 1 + y)) {
                    glCallList(meshDrawList);
                }

                glPopMatrix();
            }
        }

    glPopMatrix();

    glEndList();

    compiled[num] = true;
}

void
Brick::onTick(float deltaTime) {
}

void
Brick::renderChunk(int num) {
    Camera& camera = game.getPlayer().getCamera();

    float light1_position[] = {camera.getX(), camera.getY(), camera.getZ(), 1.0f};

    glLightfv(GL_LIGHT1, GL_POSITION, light1_position);

    glEnable(GL_FOG);
    glEnable(GL_LIGHTING);

    MazeRenderer::renderChunk(num);

    glDisable(GL_LIGHTING);
    glDisable(GL_FOG);

    glTranslatef(camera.getX(), camera.getY(), camera.getZ());
    skybox.render();
    glTranslatef(-camera.getX(), -camera.getY(), -camera.getZ());
}

}
}
