#ifndef LIBPX_EDITOR_COLOR_EDIT_HPP
#define LIBPX_EDITOR_COLOR_EDIT_HPP

#include <imgui.h>

namespace px {

/// A four channel color editor,
/// with a couple of additional variables
/// to distinguish between the first
/// and last change to a color.
class ColorEdit4 final
{
  /// If true, indicates that a change to the color
  /// value was the first change.
  bool first = false;
  /// If true, indicates that a change to the color
  /// value was the last change.
  bool committed = false;
  /// If true, it means the color edit is in the
  /// middle of being changed.
  bool changing = false;
public:
  /// Calls the color editor.
  ///
  /// @param label The label to give the editor.
  /// @param color The color being edited.
  /// @param flags Flags used to customize the color editor.
  ///
  /// @return True if the color was changed, false if it was not.
  bool operator () (const char* label, float* color, ImGuiColorEditFlags flags = 0);
  /// Indicates if a change is the first change
  /// to the color. This happens right has the user
  /// first clicks the color edit.
  ///
  /// @return True if this is the first change, false if it is not.
  inline bool isFirst() const noexcept
  {
    return first;
  }
  /// Indicates whether or not the color
  /// has been committed to its value.
  ///
  /// @note This should be checked
  /// wether or not the color value changed.
  ///
  /// @return True if the color was committed,
  /// false otherwise.
  inline bool isCommitted() const noexcept
  {
    return committed;
  }
};

} // namespace px

#endif // LIBPX_EDITOR_COLOR_EDIT_HPP
