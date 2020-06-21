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

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

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

    glfwSetKeyCallback(window, &keyCallback);

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
  /// Gets the size of the window, in pixels.
  void getWindowSize(std::size_t* w, std::size_t* h) override
  {
    int wTmp = 0;
    int hTmp = 0;

    glfwGetFramebufferSize(window, &wTmp, &hTmp);

    // Probably not going to happen but just in case.
    wTmp = std::max(0, wTmp);
    hTmp = std::max(0, hTmp);

    *w = std::size_t(wTmp);
    *h = std::size_t(hTmp);
  }
  /// Runs the application loop.
  ///
  /// @return True on success, false on failure.
  bool loop(int argc, char** argv)
  {
    if (!app->parseArgs(argc, argv)) {
      return false;
    }

    glfwMakeContextCurrent(window);

    while (!glfwWindowShouldClose(window)) {

      ImGui_ImplOpenGL3_NewFrame();

      ImGui_ImplGlfw_NewFrame();

      ImGui::NewFrame();

      auto& io = ImGui::GetIO();

      glViewport(0, 0, io.DisplaySize.x, io.DisplaySize.y);

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
  /// Passes a keyboard event to the app.
  void key(const px::KeyEvent& event)
  {
    if (ImGui::GetIO().WantCaptureKeyboard) {
      return;
    }

    app->key(event);
  }
  /// Passes a mouse button event to the application.
  void mouseButton(const px::MouseButtonEvent& button)
  {
    if (ImGui::GetIO().WantCaptureMouse) {
      // Make sure we don't step on ImGui's toes.
      return;
    }

    app->mouseButton(button);
  }
  /// Passes a mouse motion event to the application.
  void mouseMotion(const px::MouseMotionEvent& motion)
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

void keyCallback(GLFWwindow* window, int key, int /* scancode */, int action, int mods)
{
  auto* platform = (GlfwPlatform*) glfwGetWindowUserPointer(window);

  px::KeyEvent keyEvent;

  keyEvent.state = (action == GLFW_PRESS);
  keyEvent.alt   = !!(mods & GLFW_MOD_ALT);
  keyEvent.ctrl  = !!(mods & GLFW_MOD_CONTROL);
  keyEvent.shift = !!(mods & GLFW_MOD_SHIFT);

  switch (key) {
    case GLFW_KEY_O:
      keyEvent.key = 'o';
      break;
    case GLFW_KEY_S:
      keyEvent.key = 's';
      break;
    case GLFW_KEY_X:
      keyEvent.key = 'x';
      break;
    case GLFW_KEY_Y:
      keyEvent.key = 'y';
      break;
    case GLFW_KEY_Z:
      keyEvent.key = 'z';
      break;
    case GLFW_KEY_EQUAL:
      keyEvent.key = '+';
      break;
    case GLFW_KEY_MINUS:
      keyEvent.key = '-';
      break;
    default:
      // Do not bother with keys that
      // wont be used.
      return;
  }

  platform->key(keyEvent);
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mod)
{
  auto* platform = (GlfwPlatform*) glfwGetWindowUserPointer(window);

  px::MouseButtonEvent mouseButton;

    // According to GLFW doc, the 'action'
    // is one of GLFW_PRESS or GLFW_RELEASE.
    // Therefore, we can just use boolean logic here.
  mouseButton.state = (action == GLFW_PRESS);
  mouseButton.alt   = !!(mod & GLFW_MOD_ALT);
  mouseButton.ctrl  = !!(mod & GLFW_MOD_CONTROL);
  mouseButton.shift = !!(mod & GLFW_MOD_SHIFT);

  switch (button) {
    case GLFW_MOUSE_BUTTON_LEFT:
      mouseButton.id = px::MouseButtonEvent::ID::Left;
      break;
    case GLFW_MOUSE_BUTTON_RIGHT:
      mouseButton.id = px::MouseButtonEvent::ID::Right;
      break;
    case GLFW_MOUSE_BUTTON_MIDDLE:
      mouseButton.id = px::MouseButtonEvent::ID::Middle;
      break;
  }

  platform->mouseButton(mouseButton);
}

void mouseMotionCallback(GLFWwindow* window, double x, double y)
{
  auto* platform = (GlfwPlatform*) glfwGetWindowUserPointer(window);

  px::MouseMotionEvent mouseMotion;
  mouseMotion.x = int(x);
  mouseMotion.y = int(y);

  platform->mouseMotion(mouseMotion);
}

} // namespace
