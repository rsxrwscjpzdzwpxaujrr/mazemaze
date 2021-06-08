/*
 * Copyright (c) 2018-2021, Мира Странная <rsxrwscjpzdzwpxaujrr@yahoo.com>
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

#include <random>
#include <stack>

#include <SFML/System/Vector2.hpp>

namespace mazemaze {

class Chunk;

class Maze {
public:
    struct Generator {
        Generator(short x, short y);
        Generator(short x, short y, int side);

        unsigned short x;
        unsigned short y;
        unsigned char tried;
    };


    explicit Maze(int width, int height);
    ~Maze();

    bool generate(unsigned int seed);
    void cancel_generation();
    float get_generation_progress() const;

    void set_exit_x(int exit_x);
    void set_exit_y(int exit_y);
    void set_start_x(int start_x);
    void set_start_y(int start_y);

    bool get_opened(int x, int y);
    int get_width() const;
    int get_height() const;
    unsigned int get_seed() const;
    int get_exit_x() const;
    int get_exit_y() const;
    int get_start_x() const;
    int get_start_y() const;
    Chunk* get_chunks() const;
    int get_chunks_count() const;
    unsigned int get_chunks_x() const;
    unsigned int get_chunks_y() const;

    void set_seed(unsigned int seed);

    void init_chunks();

private:
    void set_opened(int x, int y, bool opened);
    void gen_exit(std::mt19937& random);
    void gen_start(std::mt19937& random);
    bool gen_step(std::stack<Generator>& generator, Generator* current_generator, int side);

    int angles_opened;
    bool need_cancel;

    int exit_x;
    int exit_y;
    int start_x;
    int start_y;
    int width;
    int height;
    unsigned int seed;
    unsigned int chunks_x;
    unsigned int chunks_y;
    Chunk* chunks;
};

}
