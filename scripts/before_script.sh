#!/bin/bash

set -x

echo "deb http://us.archive.ubuntu.com/ubuntu/ disco universe" >> /etc/apt/sources.list
sudo apt update
sudo apt install libsfml-dev
git clone https://github.com/TankOs/SFGUI.git
cd SFGUI
sed '138{s/^/#/}' CMakeLists.txt > tmp    # hack to disable SFGUI warnings
mv tmp CMakeLists.txt                     #
mkdir build
cd build
cmake -DSFGUI_BUILD_EXAMPLES=OFF ..
cmake --build .
sudo make install
cd ../..

set +x
