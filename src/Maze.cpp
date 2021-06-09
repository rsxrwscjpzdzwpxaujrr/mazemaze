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

#include "Maze.hpp"

#include <stdexcept>

#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>

#include "Chunk.hpp"
#include "Logger.hpp"
#include "utils.hpp"

namespace mazemaze {

Maze::Maze(Point2i size) :
        m_chunks(nullptr) {
    if (size.x < 1 || size.x < 1)
        throw std::invalid_argument("Width and height must be 1 or bigger");

    m_size.x = size.x * 2 + 1;
    m_size.y = size.y * 2 + 1;

    m_chunks_count.x = m_size.x / Chunk::SIZE;
    m_chunks_count.y = m_size.y / Chunk::SIZE;

    if (size.x % Chunk::SIZE != 0)
        m_chunks_count.x++;

    if (size.y % Chunk::SIZE != 0)
        m_chunks_count.y++;
}

Maze::~Maze() {
    delete [] m_chunks;
}

bool
Maze::gen_step(std::stack<Generator>& generators, Generator* generator, int side) {
    int x;
    int y;

    side_to_coords(side, x, y);

    int newx = generator->x + x * 2;
    int newy = generator->y + y * 2;

    bool inbound = newx >= 0 && newx < m_size.x && newy >= 0 && newy < m_size.y;

    if (!inbound || get_opened(newx, newy)) {
        generator->tried |= 1 << side;

        return false;
    } else {
        set_opened(generator->x + x, generator->y + y, true);
        set_opened(newx, newy, true);
        angles_opened++;

        generators.emplace(Generator(newx, newy, side));

        return true;
    }
}

unsigned int
Maze::seed() const {
    return m_seed;
}

bool
Maze::generate(unsigned int seed) {
    Logger::inst().log_debug(fmt("Maze generation started. Size is %dx%d. Seed is %d.",
                                 (m_size.x - 1) / 2,
                                 (m_size.y - 1) / 2,
                                 seed));
    init_chunks();

    angles_opened = 0;
    need_cancel = false;

    std::stack<Generator> generators;
    generators.emplace(Generator(1, 1));
    Generator* current_generator = &generators.top();

    set_opened(current_generator->x, current_generator->y, true);

    angles_opened++;

    std::mt19937 rand_gen(seed);
    std::uniform_int_distribution<> side_distrib(0, 3);

    gen_start(rand_gen);
    gen_exit(rand_gen);

    bool done = false;

    sf::Clock clock;
    sf::Time last_time = clock.getElapsedTime();

    while (!done) {
        bool goBack = current_generator->tried == 0xf;

        if (goBack) {
            generators.pop();

            if (generators.empty()) {
                done = true;
                break;
            }

            current_generator = &generators.top();
        } else {
            int side = side_distrib(rand_gen);

            while (current_generator->tried & (1 << side)) {
                side = side_distrib(rand_gen);
            }

            if (gen_step(generators, current_generator, side)) {
                current_generator = &generators.top();
            }
        }

        if (clock.getElapsedTime() - last_time >= sf::milliseconds(1000)) {
            Logger::inst().log_debug(fmt("Progress: %.1f%%", get_generation_progress() * 100.0f));
            last_time = clock.getElapsedTime();
        }

        if (need_cancel) {
            need_cancel = false;
            angles_opened = 0;

            Logger::inst().log_warn("Maze generation canceled.");
            return false;
        }
    }

    set_opened(m_exit.x, m_exit.y, true);

    m_seed = seed;

    Logger::inst().log_status(fmt("Maze generation completed. "
                                  "It took %.2f sec",
                                  clock.getElapsedTime().asMilliseconds() / 1000.0f));
    return true;
}

void
Maze::cancel_generation() {
    need_cancel = true;
}

float
Maze::get_generation_progress() const {
    return angles_opened / static_cast<float>(((m_size.x - 1) / 2) * ((m_size.y - 1) / 2));
}

bool
Maze::get_opened(Point2i point) {
    return get_opened(point.x, point.y);
}

bool
Maze::get_opened(Pointf point) {
    if (point.y > 1.0f || point.y < 0.0f)
        return true;

    return get_opened(point.x, point.z);
}

bool
Maze::get_opened(int x, int y) {
    if (!m_chunks || x < 0 || x >= m_size.x || y < 0 || y >= m_size.y)
        return true;

    unsigned int ux = static_cast<unsigned int>(x);
    unsigned int uy = static_cast<unsigned int>(y);

    return m_chunks[((uy / Chunk::SIZE) * m_chunks_count.x) + (ux / Chunk::SIZE)]
           .get_opened(ux % Chunk::SIZE, uy % Chunk::SIZE);
}

void
Maze::set_opened(int x, int y, bool opened) {
    if (!m_chunks)
        return;

    unsigned int ux = static_cast<unsigned int>(x);
    unsigned int uy = static_cast<unsigned int>(y);

    m_chunks[((uy / Chunk::SIZE) * m_chunks_count.x) + (ux / Chunk::SIZE)]
            .set_opened(ux % Chunk::SIZE, uy % Chunk::SIZE, opened);
}

void
Maze::gen_exit(std::mt19937& random) {
    std::uniform_int_distribution<> distrib(0, 3);
    std::uniform_int_distribution<> bool_distrib(0, 1);

    do {
        int angle = distrib(random);

        m_exit.x = ((angle % 2) * (m_size.x - 3)) + 1;
        m_exit.y = ((angle / 2) * (m_size.y - 3)) + 1;
    } while (m_exit == m_start && !(m_size.x <= 3 && m_size.y <= 3));

    bool direction = bool_distrib(random);

    if (direction) {
        m_exit.x += m_exit.x == 1 ? 1 : -1;
        m_exit.y += m_exit.y == 1 ? 1 : -1;
    }
}

void
Maze::gen_start(std::mt19937& random) {
    std::uniform_int_distribution<> distrib(0, 3);

    int angle = distrib(random);

    m_start.x = ((angle % 2) * (m_size.x - 3)) + 1;
    m_start.y = ((angle / 2) * (m_size.y - 3)) + 1;
}

Chunk*
Maze::chunks() const {
    return m_chunks;
}

Point2i&
Maze::exit() {
    return m_exit;
}

Point2i&
Maze::start() {
    return m_start;
}

Point2i&
Maze::size() {
    return m_size;
}

Point2i&
Maze::chunks_count() {
    return m_chunks_count;
}

void
Maze::set_seed(unsigned int seed) {
    m_seed = seed;
}

void
Maze::init_chunks() {
    if (m_chunks)
        delete [] m_chunks;

    m_chunks = new Chunk[m_chunks_count.x * m_chunks_count.y] { Chunk() };
}

Maze::Generator::Generator(short x, short y) : x(x), y(y), tried(0) {}

Maze::Generator::Generator(short x, short y, int side) : x(x), y(y) {
    static const int oppside[4] { 1, 0, 3, 2 };

    tried = 1 << oppside[side];
}

}
