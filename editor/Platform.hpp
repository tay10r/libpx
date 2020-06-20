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
};

} // namespace px

#endif // LIBPX_EDITOR_PLATFORM_HPP
