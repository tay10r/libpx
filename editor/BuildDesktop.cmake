cmake_minimum_required(VERSION 3.8.2)

find_package(OpenGL REQUIRED COMPONENTS OpenGL)
find_package(GLEW   REQUIRED)

add_executable(pxedit_desktop WIN32
  AppStorageDesktop.cpp
  GlRenderer.hpp
  GlRenderer.cpp
  GlslDesktopShaders.hpp
  GlslDesktopShaders.cpp
  GlfwPlatform.cpp
  LocalStorageDesktop.cpp
  $<TARGET_OBJECTS:pxedit_core>)

set_target_properties(pxedit_desktop
  PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")

target_link_libraries(pxedit_desktop
  PRIVATE
    px pxedit_core
    glfw
    imgui_gl
    OpenGL::OpenGL
    GLEW::GLEW
    sago::platform_folders)

target_compile_options(pxedit_desktop PRIVATE ${px_cxxflags})

target_compile_definitions(pxedit_desktop PRIVATE PXEDIT_DESKTOP=1)

target_compile_features(pxedit_desktop PRIVATE cxx_std_17)
