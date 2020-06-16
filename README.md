# libpx

*Note:* This project is still a work in progress.

This project two things:

 - A portable C++ library for rendering pixel art
 - A corresponding GUI editor

### Why Another Pixel Art Editor?

Once thing that's usually missing from pixel art editors is an easy way to make smooth animations.
This project aims at achieving that by using a very common feature to 3D editors called transformation interpolation.
That is where a user sets the beginning and ending positions of a 3D shape and the program automatically figures out the positions in between.

### Building

The library doesn't have any special build instructions, just compile the `libpx.cpp` file.
A compiler that supports the C++14 standard is required.

Building the editor requires OpenGL 3.0. The rest of the dependencies are included as git submodules (automatically cloned.)
