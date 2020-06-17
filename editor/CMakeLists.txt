cmake_minimum_required(VERSION 3.0)

find_package(OpenGL REQUIRED COMPONENTS OpenGL)
find_package(GLEW   REQUIRED)

add_executable(pxedit WIN32
  main.cpp
  BucketTool.hpp
  BucketTool.cpp
  ColorPickerTool.hpp
  ColorPickerTool.cpp
  Dialog.hpp
  Dialog.cpp
  DrawMode.hpp
  DrawMode.cpp
  DrawState.hpp
  DrawTool.hpp
  DrawTool.cpp
  EditMode.hpp
  EditMode.cpp
  Editor.hpp
  Editor.cpp
  EllipseTool.hpp
  EllipseTool.cpp
  ExportDialog.hpp
  ExportDialog.cpp
  History.hpp
  History.cpp
  ImageIO.hpp
  ImageIO.cpp
  Mode.hpp
  PenTool.hpp
  PenTool.cpp
  PoseMode.hpp
  PoseMode.cpp
  RectTool.hpp
  RectTool.cpp
  SaveDialog.hpp
  SaveDialog.cpp
  StrokeTool.hpp
  StrokeTool.cpp)

target_link_libraries(pxedit PRIVATE px pfd glfw glm OpenGL::OpenGL imgui GLEW::GLEW stb)

set_target_properties(pxedit PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")

if(CMAKE_BUILD_TYPE STREQUAL Debug)
  target_compile_definitions(px PRIVATE PX_DEBUG)
endif(CMAKE_BUILD_TYPE STREQUAL Debug)

target_compile_options(pxedit PRIVATE ${px_cxxflags})

add_subdirectory(third-party)
