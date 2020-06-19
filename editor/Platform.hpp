#ifndef LIBPX_EDITOR_PLATFORM_HPP
#define LIBPX_EDITOR_PLATFORM_HPP

namespace px {

/// This is the interface to the host platform
/// that's running the program. Possibilities include
/// GLFW, SDL2, etc...
class Platform
{
public:
  /// Just a stub.
  virtual ~Platform() {}
  /// Renders the document onto the window.
  virtual void renderDocument() = 0;
  /// Clears the contents of the window frame.
  /// The contents are replaced with a color specified in the parameters.
  /// All color components should be between 0 and 1.
  virtual void clear(float r, float g, float b, float a) = 0;
};

} // namespace px

#endif // LIBPX_EDITOR_PLATFORM_HPP
