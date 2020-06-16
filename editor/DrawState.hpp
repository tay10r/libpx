#ifndef LIBPX_EDITOR_DRAW_STATE_HPP
#define LIBPX_EDITOR_DRAW_STATE_HPP

namespace px {

/// Contains state data that is common
/// to and shared by many tools.
struct DrawState final
{
  /// A pointer to the primary color.
  float* primaryColor = nullptr;
  /// A pointer to the current pixel size.
  int* pixelSize = nullptr;
  /// A pointer to the last known cursor
  /// position on the document image.
  int* cursor = nullptr;
};

} // namespace px

#endif // LIBPX_EDITOR_DRAW_STATE_HPP
