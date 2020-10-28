cmake_minimum_required(VERSION 3.8.2)

project(libpx)

option(LIBPX_EDITOR    "Whether or not to build the editor."               OFF)
option(LIBPX_CMD       "Whether or not to build the command line program." OFF)
option(LIBPX_TUTORIALS "Wether or not to build the tutorials."             OFF)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  set(px_cxxflags -Wall -Wextra)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  set(px_cxxflags -Wall -Wextra -Wdocumentation)
endif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")

if(EMSCRIPTEN)
  if(CMAKE_BUILD_TYPE STREQUAL Debug)
    list(APPEND px_cxxflags -g4 --emit-symbol-map)
  endif(CMAKE_BUILD_TYPE STREQUAL Debug)
  set(CMAKE_EXECUTABLE_SUFFIX .html)
endif(EMSCRIPTEN)

add_library(px libpx.hpp libpx.cpp)

target_include_directories(px PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

target_compile_options(px PRIVATE ${px_cxxflags})

target_compile_features(px PRIVATE cxx_std_14)

if(LIBPX_EDITOR)
  add_subdirectory(editor)
endif(LIBPX_EDITOR)

if(LIBPX_CMD)
  add_subdirectory(cmd)
endif(LIBPX_CMD)

if(LIBPX_TUTORIALS)
  add_subdirectory(tutorials)
endif(LIBPX_TUTORIALS)
