/*
 * Copyright (c) 2021, Мира Странная <rsxrwscjpzdzwpxaujrr@yahoo.com>
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

#include <vector>
#include <algorithm>

#include "ITickable.hpp"

namespace mazemaze {

template <typename T>
class TickableHandler : public ITickable<T>{
public:
    TickableHandler();

    void addTickable(ITickable<T>* tickable);
    void removeTickable(ITickable<T>* tickable);

    void tick(T t, float deltaTime) override;

private:
    std::vector<ITickable<T>*> tickables;
};

template <typename T>
TickableHandler<T>::TickableHandler() : tickables(0) {
}

template <typename T>
void
TickableHandler<T>::addTickable(ITickable<T>* tickable) {
    tickables.emplace_back(tickable);
}

template <typename T>
void
TickableHandler<T>::removeTickable(ITickable<T>* tickable) {
    tickables.erase(std::remove(tickables.begin(), tickables.end(), tickable), tickables.end());
}

template <typename T>
void
TickableHandler<T>::tick(T t, float deltaTime) {
    for (auto* tickable: tickables) {
        tickable->tick(t, deltaTime);
    }
}

}
