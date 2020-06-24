cmake_minimum_required(VERSION 3.8.2)

add_executable(pxedit_browser
  GlRenderer.hpp
  GlRenderer.cpp
  GlslBrowserShaders.hpp
  GlslBrowserShaders.cpp
  AppStorageBrowser.cpp
  LocalStorageBrowser.cpp
  SdlPlatform.cpp
  $<TARGET_OBJECTS:pxedit_core>)

target_link_libraries(pxedit_browser
  PRIVATE
    px
    pxedit_core
    imgui_sdl)

target_compile_options(pxedit_browser PRIVATE ${px_cxxflags})

target_compile_definitions(pxedit_browser PRIVATE PXEDIT_BROWSER=1)

target_compile_features(pxedit_browser PRIVATE cxx_std_17)

target_link_options(pxedit_browser PRIVATE
  "SHELL: -s USE_SDL=2"
  "SHELL: -s EXPORTED_FUNCTIONS=\"['_pxEditAppStorageSync', '_main']\""
  "SHELL: -s EXPORTED_RUNTIME_METHODS=\"['ccall', 'cwrap']\""
  -lidbfs.js
  --pre-js "${CMAKE_CURRENT_SOURCE_DIR}/pxedit.js"
  --shell-file "${CMAKE_CURRENT_SOURCE_DIR}/pxedit.html")
