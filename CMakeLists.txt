cmake_minimum_required(VERSION 3.0)

project(libpx)

option(LIBPX_EDITOR    "Whether or not to build the editor."               OFF)
option(LIBPX_CMD       "Whether or not to build the command line program." OFF)
option(LIBPX_TUTORIALS "Wether or not to build the tutorials."             OFF)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  set(px_cxxflags -Wall -Wextra -Werror -Wfatal-errors -std=c++14)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  set(px_cxxflags -Wall -Wextra -Werror -Wfatal-errors -std=c++14)
endif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")

add_library(px libpx.hpp libpx.cpp)

target_include_directories(px PUBLIC "${CMKAE_CURRENT_SOURCE_DIR}")

target_compile_options(px PRIVATE ${px_cxxflags})

if(LIBPX_EDITOR)
  add_subdirectory(editor)
endif(LIBPX_EDITOR)

if(LIBPX_CMD)
  add_subdirectory(cmd)
endif(LIBPX_CMD)

if(LIBPX_TUTORIALS)
  add_subdirectory(tutorials)
endif(LIBPX_TUTORIALS)
