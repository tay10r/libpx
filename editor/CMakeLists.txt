cmake_minimum_required(VERSION 3.0)

# Setup the core application target.
# This implements the UI and all of the logic.

add_library(pxedit_core OBJECT
  App.hpp
  App.cpp)

target_compile_options(pxedit_core PRIVATE ${px_cxxflags})

target_link_libraries(pxedit_core PRIVATE imgui)

# Setup the OpenGL/GLFW bindings.
# This implements the platform specific code
# for OpenGL and GLFW.

find_package(OpenGL REQUIRED COMPONENTS OpenGL)
find_package(GLEW   REQUIRED)

add_executable(pxedit_gl WIN32
  Platform.hpp
  glPlatform.cpp
  $<TARGET_OBJECTS:pxedit_core>)

target_compile_options(pxedit_gl PRIVATE ${px_cxxflags})

target_link_libraries(pxedit_gl PRIVATE px glfw imgui_gl OpenGL::OpenGL GLEW::GLEW)

set_target_properties(pxedit_gl
  PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")

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
  EraserTool.hpp
  EraserTool.cpp
  ExportDialog.hpp
  ExportDialog.cpp
  Fs.hpp
  Fs.cpp
  History.hpp
  History.cpp
  ImageIO.hpp
  ImageIO.cpp
  Mode.hpp
  OpenDialog.hpp
  OpenDialog.cpp
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

target_link_libraries(pxedit PRIVATE px pfd glfw glm OpenGL::OpenGL imgui_gl GLEW::GLEW stb)

set_target_properties(pxedit PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")

if(CMAKE_BUILD_TYPE STREQUAL Debug)
  target_compile_definitions(px PRIVATE PX_DEBUG)
endif(CMAKE_BUILD_TYPE STREQUAL Debug)

target_compile_options(pxedit PRIVATE ${px_cxxflags})

add_subdirectory(third-party)
