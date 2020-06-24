cmake_minimum_required(VERSION 3.8.2)

add_subdirectory(third-party)

# Setup the core application target.
# This implements the UI and all of the logic.

add_library(pxedit_core OBJECT
  App.hpp
  App.cpp
  AppState.hpp
  AppState.cpp
  AppStorage.hpp
  AppStorage.cpp
  BrowseDocumentsState.hpp
  BrowseDocumentsState.cpp
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
  ImageIO.hpp
  ImageIO.cpp
  Index.hpp
  Index.cpp
  InternalErrorState.hpp
  InternalErrorState.cpp
  MenuBar.hpp
  MenuBar.cpp
  OpenErrorState.hpp
  OpenErrorState.cpp
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

if(EMSCRIPTEN)
  target_compile_definitions(pxedit_core PUBLIC PXEDIT_BROWSER=1)
else(EMSCRIPTEN)
  target_compile_definitions(pxedit_core PUBLIC PXEDIT_DESKTOP=1)
endif(EMSCRIPTEN)

target_compile_features(pxedit_core PRIVATE cxx_std_17)

target_compile_options(pxedit_core PRIVATE ${px_cxxflags})

target_link_libraries(pxedit_core
  PRIVATE
    nlohmann_json::nlohmann_json
    imgui
    px
    glm
    stb)

if(EMSCRIPTEN)
  include(BuildBrowser.cmake)
else(EMSCRIPTEN)
  include(BuildDesktop.cmake)
endif(EMSCRIPTEN)
