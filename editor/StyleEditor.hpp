#ifndef LIBPX_EDITOR_STYLE_EDITOR_HPP
#define LIBPX_EDITOR_STYLE_EDITOR_HPP

namespace px {

/// Used for editing the colors used
/// to display the application.
class StyleEditor final
{
  /// The background color of the window.
  float backgroundColor[4] = { 1.0, 1.0, 1.0, 1.0 };
  /// The base color of the checkerboard pattern.
  float checkerboardColor[4] { 1.0, 1.0, 1.0, 1.0 };
  /// The contrast of the checkerboard grid.
  float checkerboardContrast { 0.9 };
public:
  /// Enumerates the events that can
  /// occur from the style editor.
  enum class Event
  {
    ChangedBackgroundColor,
    ChangedCheckerboardColor,
    ChangedCheckerboardContrast
  };
  /// Used for listening to events
  /// that can occur from the stye editor.
  class Observer
  {
  public:
    /// Just a stub.
    virtual ~Observer() {}
    /// Observes an event from the style editor.
    virtual void observe(Event) {}
  };
  /// Renders a frame of the style editor.
  ///
  /// @param observer A pointer to the observer
  /// to listen for events with.
  void frame(Observer* observer = nullptr);
  /// Accesses the background color.
  ///
  /// @return A pointer to the background color.
  inline const float* getBackgroundColor() const noexcept
  {
    return backgroundColor;
  }
  /// Accesses the checkerboard color.
  ///
  /// @return A pointer to the checkerboard color.
  inline const float* getCheckerboardColor() const noexcept
  {
    return checkerboardColor;
  }
  /// Accesses the checkerboard contrast.
  ///
  /// @return The checkerboard contrast.
  inline float getCheckerboardContrast() const noexcept
  {
    return checkerboardContrast;
  }
};

} // namespace px

#endif // LIBPX_EDITOR_STYLE_EDITOR_HPP
