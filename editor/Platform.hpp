#ifndef LIBPX_EDITOR_PLATFORM_HPP
#define LIBPX_EDITOR_PLATFORM_HPP

#include <cstddef>

namespace px {

class Renderer;

/// This is the interface to the host platform
/// that's running the program. Possibilities include
/// GLFW, SDL2, etc...
class Platform
{
public:
  /// Just a stub.
  virtual ~Platform() {}
  /// Gets the renderer used by the platform.
  /// This can be used to paint the document result
  /// and various other things.
  ///
  /// @return A pointer to the renderer used by the platform.
  virtual Renderer* getRenderer() noexcept = 0;
  /// Gets the size of the window.
  ///
  /// @param w A pointer to the variable to receive the width, in pixels.
  /// @param h A pointer to the variable to receive the height, in pixels.
  virtual void getWindowSize(std::size_t* w, std::size_t* h) = 0;
};

} // namespace px

#endif // LIBPX_EDITOR_PLATFORM_HPP
