#include "Platform.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "App.hpp"
#include "GlRenderer.hpp"
#include "Input.hpp"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <memory>

#include <cstdio>
#include <cstdlib>

namespace {

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mod);

void mouseMotionCallback(GLFWwindow* window, double x, double y);

/// Implements the platform interface for OpenGL.
class GlfwPlatform final : public px::Platform
{
  /// Whether or not the global data was initialized.
  bool globalInitialized = false;
  /// Whether or not ImGui was initialized.
  bool imguiInitialized = false;
  /// A pointer to the window running the application.
  GLFWwindow* window = nullptr;
  /// The renderer that displays the document image onto the window.
  px::GlRenderer renderer;
  /// A pointer to the app instance.
  std::unique_ptr<px::App> app;
public:
  /// Constructs a new instance of the GLFW platform.
  GlfwPlatform()
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

    glfwSetWindowUserPointer(window, this);

    glfwSetCursorPosCallback(window, &mouseMotionCallback);

    glfwSetMouseButtonCallback(window, &mouseButtonCallback);

    glfwMaximizeWindow(window);

    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) {
      return;
    }

    if (!renderer.init()) {
      return;
    }

    IMGUI_CHECKVERSION();

    ImGui::CreateContext();

    ImGui_ImplGlfw_InitForOpenGL(window, true);

    ImGui_ImplOpenGL3_Init("#version 330");

    imguiInitialized = true;

    app.reset(px::App::init(this));
  }
  /// Releases memory allocated by the GLFW data.
  ~GlfwPlatform()
  {
    app.reset();

    if (imguiInitialized) {
      ImGui_ImplOpenGL3_Shutdown();
      ImGui_ImplGlfw_Shutdown();
      ImGui::DestroyContext();
      imguiInitialized = false;
    }

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
  /// Gets a pointer to the renderer to
  /// be used by the application.
  px::Renderer* getRenderer() noexcept override
  {
    return &renderer;
  }
  /// Runs the application loop.
  ///
  /// @return True on success, false on failure.
  bool loop(int argc, char** argv)
  {
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
  /// Passes a mouse button event to the application.
  void mouseButton(const px::MouseButton& button)
  {
    if (ImGui::GetIO().WantCaptureMouse) {
      // Make sure we don't step on ImGui's toes.
      return;
    }

    app->mouseButton(button);
  }
  /// Passes a mouse motion event to the application.
  void mouseMotion(const px::MouseMotion& motion)
  {
    app->mouseMotion(motion);
  }
};

} // namespace

int main(int argc, char** argv)
{
  GlfwPlatform platform;

  auto success = platform.loop(argc, argv);

  return success ? EXIT_SUCCESS : EXIT_FAILURE;
}

namespace {

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mod)
{
  auto* platform = (GlfwPlatform*) glfwGetWindowUserPointer(window);

  px::MouseButton mouseButton {
    px::MouseButton::ID::Middle,
    // According to GLFW doc, the 'action'
    // is one of GLFW_PRESS or GLFW_RELEASE.
    // Therefore, we can just use boolean logic here.
    action == GLFW_PRESS,
    !!(mod & GLFW_MOD_ALT),
    !!(mod & GLFW_MOD_CONTROL),
    !!(mod & GLFW_MOD_SHIFT)
  };

  switch (button) {
    case GLFW_MOUSE_BUTTON_LEFT:
      mouseButton.id = px::MouseButton::ID::Left;
      break;
    case GLFW_MOUSE_BUTTON_RIGHT:
      mouseButton.id = px::MouseButton::ID::Right;
      break;
    case GLFW_MOUSE_BUTTON_MIDDLE:
      mouseButton.id = px::MouseButton::ID::Middle;
      break;
  }

  platform->mouseButton(mouseButton);
}

void mouseMotionCallback(GLFWwindow* window, double x, double y)
{
  auto* platform = (GlfwPlatform*) glfwGetWindowUserPointer(window);

  px::MouseMotion mouseMotion {
    int(x),
    int(y)
  };

  platform->mouseMotion(mouseMotion);
}

} // namespace
