#ifndef LIBPX_EDITOR_INPUT_HPP
#define LIBPX_EDITOR_INPUT_HPP

namespace px {

/// Represents a mouse click event.
struct MouseButton final
{
  /// Enumerates the button IDs.
  enum class ID
  {
    /// The left button ID.
    Left,
    /// The middle button ID.
    Middle,
    /// The right button ID.
    Right
  };
  /// The ID of the button that was clicked.
  ID id = ID::Left;
  /// Whether or not the mouse is being
  /// pressed or released.
  bool state = false;
  /// Whether or not 'alt' is being held.
  bool alt = false;
  /// Whether or not 'ctrl' is being held.
  bool ctrl = false;
  /// Whether or not 'shift' is being held.
  bool shift = false;
  /// Indicates whether or not this
  /// is the left mouse button.
  inline bool isLeft() const noexcept
  {
    return id == ID::Left;
  }
  /// Indicates whether or not this
  /// is the right mouse button.
  inline bool isRight() const noexcept
  {
    return id == ID::Right;
  }
  /// Indicates if the mouse is being pressed.
  inline bool isPressed() const noexcept
  {
    return state;
  }
  /// Indicates if the mouse is being released.
  inline bool isReleased() const noexcept
  {
    return !state;
  }
};

struct MouseMotion final
{
  /// The new X coordinate.
  int x = 0;
  /// The new Y coordinate.
  int y = 0;
};

} // namesapce px

#endif // LIBPX_EDITOR_INPUT_HPP
