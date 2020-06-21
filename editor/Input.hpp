#ifndef LIBPX_EDITOR_INPUT_HPP
#define LIBPX_EDITOR_INPUT_HPP

namespace px {

struct KeyEvent;
struct MouseButtonEvent;
struct MouseMotionEvent;

/// The base of any input event.
struct InputEvent
{
public:
  /// Just a stub.
  virtual ~InputEvent() {}
};

/// A keyboard input event.
struct KeyEvent final : public InputEvent
{
  /// Whether the key is being pressed or not.
  bool state = false;
  /// The key that was pressed.
  int key = 0;
  /// Whether or not the control modifier is pressed.
  bool ctrl = false;
  /// Whether or not the alt modifier is pressed.
  bool alt = false;
  /// Whether or not the shift modifier is pressed.
  bool shift = false;
  /// Indicates if this is a key with no modifiers.
  ///
  /// @param k The key to check for.
  ///
  /// @return True if it is, false otherwise.
  bool isKey(int k) const noexcept
  {
    return (key == k) && !(ctrl || alt || shift);
  }
  /// Indicates if this is a Ctrl+<Key> sequence.
  /// The other modifier bits must be off for this to evaluate true.
  bool isCtrlKey(int k) const noexcept
  {
    return (key == k) && ctrl && !alt && !shift;
  }
  /// Whether or not this is a Ctrl+Shift+<Key> sequence.
  /// The other modifier bits must be off for this to evaluate true.
  bool isCtrlShiftKey(int k) const noexcept
  {
    return (k == key) && ctrl && shift && !alt;
  }
};

/// Represents a mouse click event.
struct MouseButtonEvent final : public InputEvent
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

/// Represents an update to the mouse
/// position on the window.
struct MouseMotionEvent final : public InputEvent
{
  /// The new X coordinate.
  int x = 0;
  /// The new Y coordinate.
  int y = 0;
};

} // namesapce px

#endif // LIBPX_EDITOR_INPUT_HPP
