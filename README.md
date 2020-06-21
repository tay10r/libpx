# libpx

[![Build Status](https://travis-ci.org/tay10r/libpx.svg?branch=master)](https://travis-ci.org/tay10r/libpx)

*Note:* This project is still a work in progress.

This project is two things:

 - A portable C++ library for rendering pixel art
 - A corresponding GUI editor

Try it [here!](https://tay10r.github.io/libpx_site/editor/pxedit_sdl.html)

### Why Another Pixel Art Editor?

Once thing that's usually missing from pixel art editors is an easy way to make smooth animations.
This project aims at achieving that by using a very common feature to 3D editors called transformation interpolation.
That is where a user sets the beginning and ending positions of a 3D shape and the program automatically figures out the positions in between.
Doing this requires a very different data model than what is used in Editors like pixilart.com and piskelapp.com, who store data as rasterized images.
It requires storing geometry instead, much like vector art editors like Inkscape.
The other benefits of doing it this way, aside from better animation, is:

 - Artwork can more easily be changed in place instead of erased and started over.
 - Images can losslessly be scaled up and down.
 - Higher quality data for future tooling

Where this program is different from programs like Inkscape is that
it is made specifically for pixel art. The development effort is greatily
simplified because of this. Therefore, more time can be spent on features
that are more useful for pixel art artists.

### Building

The library doesn't have any special build instructions, just compile the `libpx.cpp` file.
A compiler that supports the C++14 standard is required.
You can also just call CMake to build it.

```
mkdir build
cd build
cmake ..
```

Building the editor requires OpenGL 3.0 and GLEW. The rest of the dependencies are included as git submodules (automatically cloned.)

On Ubuntu, you can install this two dependencies with:

```bash
sudo apt install freeglut3-dev libglew-dev
```

To build the editor, you'll need to pass `-DLIBPX_EDITOR=ON` to CMake.

```
mkdir build
cd build
cmake .. -DLIBPX_EDITOR=ON
```
