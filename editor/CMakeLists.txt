cmake_minimum_required(VERSION 3.0)

add_subdirectory(third-party)

# Setup the core application target.
# This implements the UI and all of the logic.

add_library(pxedit_core OBJECT
  App.hpp
  App.cpp
  AppState.hpp
  AppState.cpp
  Blob.hpp
  Blob.cpp
  BucketTool.hpp
  BucketTool.cpp
  ColorPickerTool.hpp
  ColorPickerTool.cpp
  DocumentProperties.hpp
  DocumentProperties.cpp
  DrawPanel.hpp
  DrawPanel.cpp
  DrawState.hpp
  DrawState.cpp
  DrawTool.hpp
  DrawTool.cpp
  EllipseTool.hpp
  EllipseTool.cpp
  EraserTool.hpp
  EraserTool.cpp
  Fs.hpp
  Fs.cpp
  History.hpp
  History.cpp
  MenuBar.hpp
  MenuBar.cpp
  ImageIO.hpp
  ImageIO.cpp
  LayerPanel.hpp
  LayerPanel.cpp
  Log.hpp
  Log.cpp
  PenTool.hpp
  PenTool.cpp
  RectTool.hpp
  RectTool.cpp
  StrokeTool.hpp
  StrokeTool.cpp
  StyleEditor.hpp
  StyleEditor.cpp)

target_compile_options(pxedit_core PRIVATE ${px_cxxflags})

target_link_libraries(pxedit_core PRIVATE imgui px glm stb)

if(EMSCRIPTEN)

  add_executable(pxedit_sdl
    Platform.hpp
    GlRenderer.hpp
    GlRenderer.cpp
    SdlPlatform.cpp
    Shaders.hpp
    Shaders.cpp
    LocalStorage.hpp
    LocalStorageBrowser.cpp
    $<TARGET_OBJECTS:pxedit_core>)

  target_compile_options(pxedit_sdl PRIVATE ${px_cxxflags} -s USE_SDL=2)

  target_link_options(pxedit_sdl PRIVATE
    -s USE_SDL=2
    --pre-js "${CMAKE_CURRENT_SOURCE_DIR}/js/pxedit.js"
    --shell-file "${CMAKE_CURRENT_SOURCE_DIR}/shell.html")

  target_link_libraries(pxedit_sdl PRIVATE px imgui_sdl)

endif(EMSCRIPTEN)

if(EMSCRIPTEN)
  return()
endif(EMSCRIPTEN)

# Setup the OpenGL/GLFW bindings.
# This implements the platform specific code
# for OpenGL and GLFW.

find_package(OpenGL REQUIRED COMPONENTS OpenGL)
find_package(GLEW   REQUIRED)

add_executable(pxedit_glfw WIN32
  Platform.hpp
  GlfwPlatform.cpp
  GlRenderer.hpp
  GlRenderer.cpp
  Shaders.hpp
  Shaders.cpp
  LocalStorage.hpp
  LocalStorageNative.cpp
  $<TARGET_OBJECTS:pxedit_core>)

target_compile_options(pxedit_glfw PRIVATE ${px_cxxflags})

target_link_libraries(pxedit_glfw PRIVATE px glfw imgui_gl OpenGL::OpenGL GLEW::GLEW)

set_target_properties(pxedit_glfw
  PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
