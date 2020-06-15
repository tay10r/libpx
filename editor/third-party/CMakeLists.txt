cmake_minimum_required(VERSION 3.0)

if(SUBMODULES_CLONED)

  find_package(Git REQUIRED)

  execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init)

  set(SUBMODULES_CLONED TRUE CACHE BOOL "Whether or not the Git submodules have been cloned.")

endif(SUBMODULES_CLONED)

add_subdirectory(glfw)
add_subdirectory(glm)

add_library(imgui
  imgui/imgui.cpp
  imgui/imgui_draw.cpp
  imgui/imgui_widgets.cpp
  imgui/examples/imgui_impl_glfw.cpp
  imgui/examples/imgui_impl_opengl3.cpp)
target_include_directories(imgui PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/imgui")
target_include_directories(imgui PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/imgui/examples")

add_library(stb INTERFACE)
target_include_directories(stb INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/stb")

add_library(pfd INTERFACE)
target_include_directories(pfd INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/portable-file-dialogs")
