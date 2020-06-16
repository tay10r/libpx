cmake_minimum_required(VERSION 3.0)

find_package(OpenGL REQUIRED COMPONENTS OpenGL)
find_package(GLEW   REQUIRED)

add_executable(pxedit WIN32
  main.cpp
  BucketTool.hpp
  BucketTool.cpp
  Dialog.hpp
  Dialog.cpp
  DrawMode.hpp
  DrawMode.cpp
  EditMode.hpp
  EditMode.cpp
  Editor.hpp
  Editor.cpp
  ExportDialog.hpp
  ExportDialog.cpp
  Mode.hpp
  PenTool.hpp
  PenTool.cpp
  PoseMode.hpp
  PoseMode.cpp
  StrokeTool.hpp
  StrokeTool.cpp
  Tool.hpp
  ImageIO.hpp
  ImageIO.cpp)

target_link_libraries(pxedit PRIVATE px pfd glfw glm OpenGL::OpenGL imgui GLEW::GLEW stb)

set_target_properties(pxedit PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")

if(CMAKE_BUILD_TYPE STREQUAL Debug)
  target_compile_definitions(px PRIVATE PX_DEBUG)
endif(CMAKE_BUILD_TYPE STREQUAL Debug)

target_compile_options(pxedit PRIVATE ${px_cxxflags})

add_subdirectory(third-party)
