#ifndef LIBPX_EDITOR_RENDERER_HPP
#define LIBPX_EDITOR_RENDERER_HPP

namespace px {

struct Document;

/// This is the base class of a renderer.
/// A renderer for the editor is used to
/// paint the checkered background, blit the
/// image onto the window, and highlight any
/// selected points or lines.
class Renderer
{
public:
  /// Just a stub.
  virtual ~Renderer() {}
  /// Blits the document image onto the window.
  ///
  /// @param img A pointer to the RGBA image data.
  /// @param w The width of the image, in pixels.
  /// @param h The height of the image, in pixels.
  virtual void blit(const float* img, std::size_t w, std::size_t h) = 0;
  /// Clears the window background.
  ///
  /// @note The RGB components should not be premultiplied.
  virtual void clear(float r, float g, float b, float a) = 0;
  /// Clears the window background using a color pointer.
  ///
  /// @note The RGB components should not be premultiplied.
  void clear(const float* rgba)
  {
    clear(rgba[0], rgba[1], rgba[2], rgba[3]);
  }
  /// Sets the checkerboard color using a color pointer.
  ///
  /// @note The RGB components should not be premultiplied.
  void setCheckerboardColor(const float* rgba)
  {
    setCheckerboardColor(rgba[0], rgba[1], rgba[2], rgba[3]);
  }
  /// Sets the base color of the checkerboard background.
  ///
  /// @note The RGB components should not be premultiplied.
  virtual void setCheckerboardColor(float r, float g, float b, float a) = 0;
  /// Sets the checkerboard contrast level.
  /// This should be between 0 and 1.
  virtual void setCheckerboardContrast(float contrast) = 0;
  /// Sets the cursor position, in terms of document coordinates.
  /// This is used to update the highlighted pixel.
  virtual void setCursor(int x, int y) = 0;
  /// Sets the transform to apply to the image.
  ///
  /// @param transform The column-major 4x4 transformation to assign.
  virtual void setTransform(const float* transform) = 0;
};

} // namespace px

#endif // LIBPX_EDITOR_RENDERER_HPP
