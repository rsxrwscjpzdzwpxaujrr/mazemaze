#!/bin/bash

set -xeE
trap 'sleep 1' ERR

mkdir build
cd build
export CMAKE_MAZEMAZE_FLAGS="
    -DCMAKE_VERBOSE_MAKEFILE=TRUE  \
    -DSFGUI_SUBMODULE=FALSE"
if [[ $WINDOWS = "TRUE" ]]
then
    CMAKE_MAZEMAZE_FLAGS="$CMAKE_MAZEMAZE_FLAGS  \
        -DSFML_STATIC_LIBRARIES=TRUE  \
        -DMAZEMAZE_INSTALL_DIR=${MXE_DIR}/usr/$MXE_TARGET/opt"
fi
$CMAKE $CMAKE_MAZEMAZE_FLAGS ..
$CMAKE --build .
sudo make install
cd ..

ccache -s

set +xeE
