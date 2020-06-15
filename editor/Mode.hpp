#ifndef LIBPX_EDITOR_MODE_HPP
#define LIBPX_EDITOR_MODE_HPP

namespace px {

/// A mode is a editor state that is
/// used to perform a specific class of
/// operations of the document.
class Mode
{
public:
  /// Just a stub.
  virtual ~Mode() {}
  /// Renders the mode windows.
  virtual void render() = 0;
  /// Handles a change in the left click button state.
  ///
  /// @param state The new value of the button state.
  virtual void leftClick(bool state) = 0;
  /// Handles a change in the right click button state.
  ///
  /// @param state The new value of the button state.
  virtual void rightClick(bool state) = 0;
  /// Handles mouse motion.
  ///
  /// @param x The X coordinate of the mouse, in terms of image coordinates.
  /// @param x The Y coordinate of the mouse, in terms of image coordinates.
  virtual void mouseMotion(unsigned x, unsigned y) = 0;
};

} // namespace px

#endif // LIBPX_EDITOR_MODE_HPP
