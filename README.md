Mazemaze
========
[![Build Status](https://travis-ci.org/mirai65536/mazemaze.svg?branch=master)](https://travis-ci.org/mirai65536/mazemaze)

Simple 3D OpenGL first-person maze game.

![](screenshot.png)

### Install:
You can download the latest version precompiled binaries:

##### Windows:
* [Mazemaze_0.2.2_Windows_64bit.zip](https://github.com/mirai65536/mazemaze/releases/download/v0.2.2/Mazemaze_0.2.2_Windows_64bit.zip)
* [Mazemaze_0.2.2_Windows_32bit.zip](https://github.com/mirai65536/mazemaze/releases/download/v0.2.2/Mazemaze_0.2.2_Windows_32bit.zip)

##### Ubuntu, Debian, etc:
* [mazemaze_0.2.2_amd64.deb](https://github.com/mirai65536/mazemaze/releases/download/v0.2.2/mazemaze_0.2.2_amd64.deb)

##### Other GNU/Linux:
* [Mazemaze_0.2.2_Linux_64bit.tar.gz](https://github.com/mirai65536/mazemaze/releases/download/v0.2.2/Mazemaze_0.2.2_Linux_64bit.tar.gz)
* Note that in this case, you need to install the dependencies for distribution by yourself (JsonCpp and SFML 2.5 and later)

You can check checksums of binary packages in [Travis CI logs](https://travis-ci.org/mirai65536/mazemaze/builds).

### Build:
On Arch Linux you can also use a [mazemaze](https://aur.archlinux.org/packages/mazemaze/) and [mazemaze-git](https://aur.archlinux.org/packages/mazemaze-git/) AUR packages instead.

First you need to install build dependencies:
* Ubuntu, Debian, etc:
```
$ sudo apt install libsfml-dev libjsoncpp-dev
```

* Arch Linux:
```
$ sudo pacman -S sfml jsoncpp
```

Then you can proceed with the build:
```
$ git clone https://github.com/mirai65536/mazemaze.git
$ cd mazemaze
$ mkdir build
$ cd build
$ cmake ..
$ make
```

You can also install the game after this:
```
$ sudo make install
```
