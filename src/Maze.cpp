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

Maze::Maze(int width, int height) :
        chunks(nullptr) {
    if (width < 1 || height < 1)
        throw std::invalid_argument("Width and height must be 1 or bigger");

    Maze::width  = width  * 2 + 1;
    Maze::height = height * 2 + 1;

    chunks_x = Maze::width  / Chunk::SIZE;
    chunks_y = Maze::height / Chunk::SIZE;

    if (width % Chunk::SIZE != 0)
        chunks_x++;

    if (height % Chunk::SIZE != 0)
        chunks_y++;
}

Maze::~Maze() {
    delete [] chunks;
}

bool
Maze::gen_step(std::stack<Generator>& generators, Generator* generator, int side) {
    int x;
    int y;

    side_to_coords(side, x, y);

    int newx = generator->x + x * 2;
    int newy = generator->y + y * 2;

    bool inbound = newx >= 0 && newx < width && newy >= 0 && newy < height;

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
Maze::get_seed() const {
    return seed;
}

bool
Maze::generate(unsigned int seed) {
    Logger::inst().log_debug(fmt("Maze generation started. Size is %dx%d. Seed is %d.",
                                 (width - 1) / 2,
                                 (height - 1) / 2,
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

    set_opened(get_exit_x(), get_exit_y(), true);

    Maze::seed = seed;

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
    return angles_opened / static_cast<float>(((width - 1) / 2) * ((height - 1) / 2));
}

void
Maze::set_exit_x(int exitX) {
    Maze::exit_x = exitX;
}

void
Maze::set_exit_y(int exitY) {
    Maze::exit_y = exitY;
}

void
Maze::set_start_x(int startX) {
    Maze::start_x = startX;
}

void
Maze::set_start_y(int startY) {
    Maze::start_y = startY;
}


bool
Maze::get_opened(int x, int y) {
    if (!chunks || x < 0 || x >= width || y < 0 || y >= height)
        return true;

    unsigned int ux = static_cast<unsigned int>(x);
    unsigned int uy = static_cast<unsigned int>(y);

    return chunks[((uy / Chunk::SIZE) * chunks_x) + (ux / Chunk::SIZE)]
           .get_opened(ux % Chunk::SIZE, uy % Chunk::SIZE);
}

void
Maze::set_opened(int x, int y, bool opened) {
    if (!chunks)
        return;

    unsigned int ux = static_cast<unsigned int>(x);
    unsigned int uy = static_cast<unsigned int>(y);

    chunks[((uy / Chunk::SIZE) * chunks_x) + (ux / Chunk::SIZE)]
            .set_opened(ux % Chunk::SIZE, uy % Chunk::SIZE, opened);
}

void
Maze::gen_exit(std::mt19937& random) {
    std::uniform_int_distribution<> distrib(0, 3);
    std::uniform_int_distribution<> bool_distrib(0, 1);

    do {
        int angle = distrib(random);

        exit_x = ((angle % 2) * (width - 3)) + 1;
        exit_y = ((angle / 2) * (height - 3)) + 1;
    } while (exit_x == start_x && exit_y == start_y && !(width <= 3 && height <= 3));

    bool direction = bool_distrib(random);

    int* directed_coord;

    if (direction)
        directed_coord = &exit_x;
    else
        directed_coord = &exit_y;

    if (*directed_coord == 1)
        (*directed_coord)--;
    else
        (*directed_coord)++;
}

void
Maze::gen_start(std::mt19937& random) {
    std::uniform_int_distribution<> distrib(0, 3);

    int angle = distrib(random);

    start_x = ((angle % 2) * (width - 3)) + 1;
    start_y = ((angle / 2) * (height - 3)) + 1;
}

int
Maze::get_width() const {
    return width;
}

int
Maze::get_height() const {
    return height;
}

int
Maze::get_exit_x() const {
    return exit_x;
}

int
Maze::get_exit_y() const {
    return exit_y;
}

int
Maze::get_start_x() const {
    return start_x;
}

int
Maze::get_start_y() const {
    return start_y;
}

Chunk*
Maze::get_chunks() const {
    return chunks;
}

int
Maze::get_chunks_count() const {
    return chunks_x * chunks_y;
}

unsigned int
Maze::get_chunks_x() const {
    return chunks_x;
}

unsigned int
Maze::get_chunks_y() const {
    return chunks_y;
}

void
Maze::set_seed(unsigned int seed) {
    Maze::seed = seed;
}

void
Maze::init_chunks() {
    if (chunks)
        delete [] chunks;

    chunks = new Chunk[get_chunks_count()] { Chunk() };
}

Maze::Generator::Generator(short x, short y) : x(x), y(y), tried(0) {}

Maze::Generator::Generator(short x, short y, int side) : x(x), y(y) {
    static const int oppside[4] { 1, 0, 3, 2 };

    tried = 1 << oppside[side];
}

}
