cmake_minimum_required(VERSION 3.0)

add_executable(pxcmd pxcmd.cpp)

target_link_libraries(pxcmd PRIVATE px)

set_target_properties(pxcmd PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
