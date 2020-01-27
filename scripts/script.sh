#!/bin/bash

mkdir build
cd build
cmake -DCMAKE_VERBOSE_MAKEFILE=ON ..
cmake --build .
sudo make install
cd ..
