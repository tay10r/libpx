cmake_minimum_required(VERSION 3.0)

add_executable(px_example example.cpp)

target_link_libraries(px_example PRIVATE px)

set_target_properties(px_example
  PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}"
    OUTPUT_NAME example)
