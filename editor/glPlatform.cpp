#include "Platform.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "App.hpp"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <memory>

#include <cstdio>
#include <cstdlib>

namespace {

/// Implements the platform interface for OpenGL.
class glPlatform final : public px::Platform
{
  /// Whether or not the global data was initialized.
  bool globalInitialized = false;
  /// A pointer to the window running the application.
  GLFWwindow* window = nullptr;
public:
  /// Constructs a new instance of the GLFW platform.
  glPlatform()
  {
    if (!glfwInit()) {
      std::fprintf(stderr, "Failed to initialize GLFW\n");
      return;
    }

    globalInitialized = true;

    window = glfwCreateWindow(640, 480, px::App::windowTitle(), nullptr, nullptr);
    if (!window) {
      std::fprintf(stderr, "Failed to create window.\n");
      return;
    }

    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) {
      return;
    }

    IMGUI_CHECKVERSION();

    ImGui::CreateContext();

    ImGui_ImplGlfw_InitForOpenGL(window, true);

    ImGui_ImplOpenGL3_Init("#version 330");
  }
  /// Releases memory allocated by the GLFW data.
  ~glPlatform()
  {
    ImGui_ImplOpenGL3_Shutdown();

    ImGui_ImplGlfw_Shutdown();

    ImGui::DestroyContext();

    if (window) {
      glfwDestroyWindow(window);
      window = nullptr;
    }

    if (globalInitialized) {
      glfwTerminate();
      globalInitialized = false;
    }
  }
  /// Indicates whether or not the initialization
  /// process failed.
  ///
  /// @return True if it failed, false otherwise.
  bool failed() noexcept
  {
    return !window;
  }
  /// Renders the document.
  void renderDocument() override {}
  /// Clears the window frame contents.
  void clear(float r, float g, float b, float a) override
  {
    glClearColor(r, g, b, a);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  }
  /// Runs the application loop.
  ///
  /// @return True on success, false on failure.
  bool loop(int argc, char** argv)
  {
    std::unique_ptr<px::App> app(px::App::init(this));

    if (!app->parseArgs(argc, argv)) {
      return false;
    }

    while (!glfwWindowShouldClose(window)) {

      ImGui_ImplOpenGL3_NewFrame();

      ImGui_ImplGlfw_NewFrame();

      ImGui::NewFrame();

      if (!app->frame()) {
        return false;
      }

      ImGui::Render();

      ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

      glfwSwapBuffers(window);

      glfwPollEvents();
    }

    return true;
  }
};

} // namespace

int main(int argc, char** argv)
{
  glPlatform platform;

  auto success = platform.loop(argc, argv);

  return success ? EXIT_SUCCESS : EXIT_FAILURE;
}
