#!/bin/bash

set -xe

export VERSION=0.2.2

if [[ $ARCH = "x86_64" ]]
then
  export ARCH_HUMAN="64bit"
  export APT_ARCH=amd64
else
  export ARCH_HUMAN="32bit"
  export APT_ARCH=i386
fi

if [[ $WINDOWS = "TRUE" ]]
then
    sudo echo "deb http://pkg.mxe.cc/repos/apt bionic main" \
        | sudo tee /etc/apt/sources.list.d/mxeapt.list

    sudo apt-key adv --keyserver keyserver.ubuntu.com \
        --recv-keys C6BF758A33A3A276

    export MXE_TARGET=$ARCH-w64-mingw32.static
    export MXE_APT_TARGET=$(echo "$MXE_TARGET" | sed 's/_/-/g')
    export MXE_DIR=/usr/lib/mxe
    export MXE_PREFIX=${MXE_DIR}/usr/$MXE_TARGET

    export CMAKE="${MXE_DIR}/usr/bin/${MXE_TARGET}-cmake \
        -DMXE_USE_CCACHE=FALSE"

    sudo apt-get --yes update
    sudo apt-get --yes \
        --no-install-suggests \
        --no-install-recommends \
        install \
        mxe-$MXE_APT_TARGET-sfml \
        mxe-$MXE_APT_TARGET-jsoncpp
    sudo apt-get --yes remove mxe-$MXE_APT_TARGET-sfml

    # Hack to fix linking error
    sudo ln -s $MXE_PREFIX/lib/libopengl32.a \
               $MXE_PREFIX/lib/libOpenGL32.a

    # SFML

    wget https://www.sfml-dev.org/files/SFML-2.5.1-sources.zip
    unzip SFML-2.5.1-sources.zip
    cd SFML-2.5.1
    mkdir build
    cd build
    $CMAKE \
        -DSFML_USE_STATIC_STD_LIBS=TRUE \
        -BUILD_SHARED_LIBS=FALSE \
        -DSFML_BUILD_EXAMPLES=FALSE \
        -DSFML_BUILD_DOC=FALSE \
        -DSFML_BUILD_AUDIO=FALSE \
        -DSFML_BUILD_GRAPHICS=TRUE \
        -DSFML_BUILD_WINDOW=TRUE \
        -DSFML_BUILD_NETWORK=FALSE \
        ..
    $CMAKE --build .
    sudo make install
    cd ../..

    # Icon
    convert data/icon.png \
            -colors 256 \
            -define icon:auto-resize=16,24,32,48,256 \
            -background transparent \
            win/icon.ico
else
    export CMAKE=cmake

    sudo echo "deb http://us.archive.ubuntu.com/ubuntu/ eoan universe" >> \
        /etc/apt/sources.list

    sudo apt-get --yes update
    sudo apt-get --yes install libsfml-dev
fi

# SFGUI

git clone https://github.com/TankOs/SFGUI.git
cd SFGUI
sed '138{s/^/#/}' CMakeLists.txt > tmp    # hack to disable SFGUI warnings
mv tmp CMakeLists.txt                     #
mkdir build
cd build
CMAKE_SFGUI_FLAGS="-DSFGUI_BUILD_EXAMPLES=FALSE  \
                   -DSFGUI_BUILD_SHARED_LIBS=FALSE"
if [[ $WINDOWS = "TRUE" ]]
then
    CMAKE_SFGUI_FLAGS="$CMAKE_SFGUI_FLAGS  \
        -DSFGUI_STATIC_STD_LIBS=TRUE  \
        -DSFML_STATIC_LIBRARIES=TRUE  \
        -DSFGUI_BUILD_EXAMPLES=FALSE"
fi
$CMAKE $CMAKE_SFGUI_FLAGS ..
$CMAKE --build .
sudo make install
cd ../..

set +xe
