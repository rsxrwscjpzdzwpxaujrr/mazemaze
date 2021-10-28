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

#include "Point.hpp"
#include "Point2.hpp"

#include <SFML/System/Vector2.hpp>

namespace mazemaze {

class Chunk;

class Maze {
public:
    explicit Maze(Point2i size);
    ~Maze();

    bool generate(unsigned int seed);
    void cancel_generation();

    float        generation_progress() const;
    bool         get_opened(Point2i point) const;
    bool         get_opened(Pointf  point) const;
    unsigned int seed        () const;
    Chunk*       chunks      () const;
    Point2i&     exit        ();
    Point2i&     start       ();
    Point2i&     size        ();
    Point2i&     chunks_count();

    void set_seed(unsigned int seed);

    void init_chunks();

private:
    struct Generator {
        Generator(short x, short y);
        Generator(short x, short y, int side);

        unsigned short x;
        unsigned short y;
        unsigned char tried;
    };

    bool get_opened(int x, int y) const;
    void set_opened(int x, int y, bool opened);

    void gen_exit(std::mt19937& random);
    void gen_start(std::mt19937& random);
    bool gen_step(std::stack<Generator>& generator, Generator* current_generator, int side);

    int angles_opened;
    bool need_cancel;

    Point2i m_exit;
    Point2i m_start;
    Point2i m_size;
    Point2i m_chunks_count;

    unsigned int m_seed;
    Chunk* m_chunks;
};

}
