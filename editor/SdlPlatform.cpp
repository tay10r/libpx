#include "Platform.hpp"

#include "App.hpp"
#include "GlRenderer.hpp"

#ifdef __EMSCRIPTEN__
#include <SDL.h>
#include <SDL_opengles2.h>
#include <emscripten.h>
#else
#include <SDL2/SDL.h>
#endif

#include <imgui.h>
#include <imgui_impl_sdl.h>
#include <imgui_impl_opengl3.h>

#include <memory>
#include <string>

#include <cstdio>
#include <cstdlib>

namespace {

/// Implements the platform interface for SDL2.
class SdlPlatform final : public px::Platform
{
  /// Whether or not SDL was initialized.
  bool sdlInitialized = false;
  /// The background color.
  float background[4] { 0, 0, 0, 0 };
  /// The OpenGL context for SDL2.
  SDL_GLContext glContext;
  /// A pointer to the application window.
  SDL_Window* window = nullptr;
  /// The renderer that blits the image onto the window.
  px::GlRenderer renderer;
  /// A pointer to the renderer instance.
  //SDL_Renderer* renderer = nullptr;
  /// A pointer to the application instance.
  std::unique_ptr<px::App> app;
public:
  /// Constructs a new instance of the SDL2 platform.
  SdlPlatform()
  {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
      return;
    }

    sdlInitialized = true;

    const char* glslVersion = "#version 100";

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    int windowFlags = 0;
    windowFlags |= SDL_WINDOW_OPENGL;
    windowFlags |= SDL_WINDOW_RESIZABLE;
    windowFlags |= SDL_WINDOW_ALLOW_HIGHDPI;
    
    window = SDL_CreateWindow(px::App::windowTitle(),
                              SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED,
                              640, 480,
                              windowFlags);
    if (!window) {
      std::fprintf(stderr, "Failed to create SDL2 window.\n");
      return;
    }

    glContext = SDL_GL_CreateContext(window);
    if (!glContext) {
      std::fprintf(stderr, "Failed to create GL context.\n");
      return;
    }

    SDL_GL_SetSwapInterval(1);

    renderer.init();

    IMGUI_CHECKVERSION();

    ImGui::CreateContext();

    ImGui_ImplSDL2_InitForOpenGL(window, glContext);

    ImGui_ImplOpenGL3_Init(glslVersion);

    app.reset(px::App::init(this));
  }
  /// Releases memory allocated by the GLFW data.
  ~SdlPlatform()
  {
    /* Just to maintain the order of init/cleanup,
     * we'll close the app first. */
    app.reset();

    if (window) {
      SDL_DestroyWindow(window);
    }

    if (sdlInitialized) {
      SDL_Quit();
    }
  }
  /// Gets a pointer to the renderer instance.
  Renderer* getRenderer() noexcept override
  {
    return &renderer;
  }
  /** Exits the application window. */
  void quit() /* TODO : override */
  {
#ifdef __EMSCRIPTEN__
    emscripten_cancel_main_loop();
#endif
  }
  /** Passes command line arguments to the app.
   *
   * @return True on success, false on failure.
   * */
  bool passArgs(int argc, char** argv)
  {
    return app->parseArgs(argc, argv);
  }
  /// Renders one frame of the application loop.
  ///
  /// @return True on success, false on failure.
  bool frame()
  {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
      ImGui_ImplSDL2_ProcessEvent(&event);
    }

    ImGui_ImplOpenGL3_NewFrame();

    ImGui_ImplSDL2_NewFrame(window);

    ImGui::NewFrame();

    auto& io = ImGui::GetIO();

    /* While ImGui Rendering doesn't occur until
     * after 'app->frame()', at any time the application
     * can call Platform::renderDocument. So the GL context
     * has to be updated before that point. */

    SDL_GL_MakeCurrent(window, glContext);

    glViewport(0, 0, io.DisplaySize.x, io.DisplaySize.y);

    glClearColor(background[0], background[1], background[2], background[3]);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    app->frame();

    ImGui::Render();

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    SDL_GL_SwapWindow(window);

    return true;
  }
};

#ifdef __EMSCRIPTEN__

/** Called from browser to render animation frame.
 *
 * @param platformPtr A pointer to the platform instance.
 * */
void emscriptenLoopCallback(void* platformPtr)
{
  SdlPlatform* platform = (SdlPlatform*)platformPtr;

  if (!platform->frame()) {
    platform->quit();
  }
}

#endif /* __EMSCRIPTEN__ */

} // namespace

int main(int argc, char** argv)
{
  SdlPlatform platform;

  if (!platform.passArgs(argc, argv)) {
    return EXIT_FAILURE;
  }

#ifdef __EMSCRIPTEN__
  emscripten_set_main_loop_arg(&emscriptenLoopCallback, &platform, -1, 1 /* block until done */);
#else /* __EMSCRIPTEN__ */
#endif /* __EMSCRIPTEN__ */

  return EXIT_SUCCESS;
}
