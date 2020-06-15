#include "Editor.hpp"

#include <GLFW/glfw3.h>

#include <GL/gl.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <cstdio>
#include <cstdlib>

int main()
{
  if (!glfwInit()) {
    std::fprintf(stderr, "Failed to initialize GLFW.\n");
    return EXIT_FAILURE;
  }

  px::Editor editor;

  if (!editor.createWindow()) {
    std::fprintf(stderr, "Failed to open window.\n");
    return EXIT_FAILURE;
  }

  if (!editor.initGlData()) {
    std::fprintf(stderr, "Failed to initialize OpenGL data.\n");
    return EXIT_FAILURE;
  }

#ifdef PX_DEBUG
  glEnable(GL_DEBUG_OUTPUT);
  glDebugMessageCallback(messageCallback, 0);
#endif /* PX_DEBUG */

  while (!editor.shouldExit()) {
    editor.iterate();
  }

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glfwTerminate();

  return EXIT_SUCCESS;
}
