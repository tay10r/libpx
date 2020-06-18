#ifndef LIBPX_EDITOR_DRAW_STATE_HPP
#define LIBPX_EDITOR_DRAW_STATE_HPP

#include <cstddef>

namespace px {

/// An interface to the state data in draw mode.
class DrawState
{
public:
  /// Just a stub.
  virtual ~DrawState() {}
  /// Gets the current pixel size.
  virtual int getPixelSize() const noexcept = 0;
  /// Gets the current cursor position.
  virtual const int* getCursor() const noexcept = 0;
  /// Gets the current primary color.
  virtual const float* getPrimaryColor() const noexcept = 0;
  /// Requires an index of the current layer.
  /// This will create a layer if there are presently
  /// none in the document.
  virtual std::size_t requireCurrentLayer() = 0;
};

} // namespace px

#endif // LIBPX_EDITOR_DRAW_STATE_HPP
