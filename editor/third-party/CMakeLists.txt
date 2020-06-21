cmake_minimum_required(VERSION 3.0)

set(GLFW_BUILD_DOCS OFF)

if(NOT SUBMODULES_CLONED)

  find_package(Git REQUIRED)

  execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init)

  set(SUBMODULES_CLONED TRUE CACHE BOOL "Whether or not the Git submodules have been cloned.")

endif(NOT SUBMODULES_CLONED)

add_library(imgui
  imgui/imgui.cpp
  imgui/imgui_draw.cpp
  imgui/imgui_demo.cpp
  imgui/imgui_widgets.cpp
  imgui/misc/cpp/imgui_stdlib.cpp)

target_include_directories(imgui PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/imgui")
target_include_directories(imgui PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/imgui/examples")
target_include_directories(imgui PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/imgui/misc/cpp")

add_subdirectory(glm)

if(EMSCRIPTEN)

  add_library(imgui_sdl
    imgui/examples/imgui_impl_sdl.cpp
    imgui/examples/imgui_impl_opengl3.cpp)

  target_link_libraries(imgui_sdl PUBLIC imgui)

  target_compile_options(imgui_sdl PUBLIC -s USE_SDL=2)

endif(EMSCRIPTEN)

if(NOT EMSCRIPTEN)

  add_subdirectory(glfw)

  add_library(imgui_gl
    imgui/examples/imgui_impl_glfw.cpp
    imgui/examples/imgui_impl_opengl3.cpp)
  target_link_libraries(imgui_gl PUBLIC imgui)

endif(NOT EMSCRIPTEN)

add_library(stb INTERFACE)
target_include_directories(stb INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/stb")

add_library(pfd INTERFACE)
target_include_directories(pfd INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/portable-file-dialogs")
