#ifndef LIBPX_EDITOR_STYLE_EDITOR_HPP
#define LIBPX_EDITOR_STYLE_EDITOR_HPP

namespace px {

/// Used for editing the colors used
/// to display the application.
class StyleEditor final
{
  /// The base color of the checkerboard pattern.
  float checkerboardColor[3] { 0.5, 0.5, 0.5 };
  /// The contrast of the checkerboard grid.
  float checkerboardContrast { 0.9 };
public:
  /// Renders a frame of the style editor.
  void frame();
};

} // namespace px

#endif // LIBPX_EDITOR_STYLE_EDITOR_HPP
