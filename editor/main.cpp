#include "Editor.hpp"

#include <GLFW/glfw3.h>

#include <GL/gl.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <string>
#include <vector>

#include <cstdio>
#include <cstdlib>
#include <cstring>

int main(int argc, char** argv)
{
  // A filename given from the command line.
  const char* filename = nullptr;

  // Parse command line arguments.

  auto isOpt = [](const char* arg, const char* s, const char* l) {
    return (std::strcmp(arg, s) == 0) || (std::strcmp(arg, l) == 0);
  };

  for (int i = 1; i < argc; i++) {
    if (isOpt(argv[i], "-h", "--help")) {
      std::printf("Usage: %s [file]\n", argv[0]);
      return EXIT_FAILURE;
    } else if (argv[i][0] != '-') {
      if (filename) {
        std::fprintf(stderr, "Invalid trailing argument '%s'\n", argv[i]);
        return EXIT_FAILURE;
      }
      filename = argv[i];
    } else {
      std::fprintf(stderr, "Unknown option '%s'\n", argv[i]);
      return EXIT_FAILURE;
    }
  }

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

  auto failed = false;

  if (filename) {
    failed = !editor.openDoc(filename);
  }

  while (!failed) {

    if (editor.shouldExit() && editor.canSafelyExit()) {
      break;
    }

    editor.iterate();
  }

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glfwTerminate();

  return EXIT_SUCCESS;
}
