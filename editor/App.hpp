#ifndef LIBPX_EDITOR_APP_HPP
#define LIBPX_EDITOR_APP_HPP

namespace px {

struct Document;
struct Image;

struct MouseButton;
struct MouseMotion;

class AppState;
class Log;
class MenuBar;
class Platform;

/// This is the interface for the application.
/// The @ref Platform and @ref App class most communicate
/// with each other via their interfaces.
class App
{
  /// A pointer to the platform that is hosting the app.
  Platform* platform = nullptr;
public:
  /// Creates a new instance of the app.
  ///
  /// @param platform A pointer to the platform
  /// that is hosting the app instance.
  ///
  /// @return A pointer to the app.
  static App* init(Platform* platform);
  /// Constructs the base interface to the app.
  ///
  /// @param p A pointer to the platform hosting the app.
  inline App(Platform* p) : platform(p) {}
  /// Just a stub.
  virtual ~App() {}
  /// Gets a pointer to the current document snapshot.
  virtual Document* getDocument() noexcept = 0;
  /// Gets a pointer to the current document snapshot.
  virtual const Document* getDocument() const noexcept = 0;
  /// Gets a pointer to the menu bar.
  virtual MenuBar* getMenuBar() noexcept = 0;
  /// Gets a pointer to the menu bar.
  virtual const MenuBar* getMenuBar() const noexcept = 0;
  /// Gets a pointer to the latest rendered image.
  virtual Image* getImage() noexcept = 0;
  /// Gets a pointer to the latest rendered image.
  virtual const Image* getImage() const noexcept = 0;
  /// Gets a pointer to the platform hosting the app.
  ///
  /// @return A pointer to the platform for non-const access.
  inline Platform* getPlatform() noexcept
  {
    return platform;
  }
  /// Gets a pointer to the platform hosting the app.
  ///
  /// @return A pointer to the platform for const access.
  inline const Platform* getPlatform() const noexcept
  {
    return platform;
  }
  /// Gets a pointer to the log.
  ///
  /// @return A pointer to the log.
  virtual Log* getLog() noexcept = 0;
  /// Called to render a frame.
  ///
  /// @return True on success, false on failure.
  virtual bool frame() = 0;
  /// This function is called by the platform
  /// implementation to notify the application of
  /// a mouse motion event.
  virtual void mouseMotion(const MouseMotion& event) = 0;
  /// This function is called by the platform
  /// implementation to notify the app of a mouse
  /// button event.
  virtual void mouseButton(const MouseButton& event) = 0;
  /// Parses arguments from the command line.
  ///
  /// @param argc The number of arguments to parse.
  /// @param argv The argument array.
  ///
  /// @return True on success, false on failure.
  virtual bool parseArgs(int argc, char** argv) = 0;
  /// Pushes an app state to the app.
  ///
  /// @param appState The app state to push.
  virtual void pushAppState(AppState* appState) = 0;
  /// Takes a snapshot of the current document.
  /// This is primarily for undo/redo operations.
  virtual void snapshotDocument() = 0;
  /// Gets the window title.
  ///
  /// @return A pointer to the window title to display.
  static constexpr const char* windowTitle() noexcept
  {
    return "PX Editor";
  }
};

} // namespace px

#endif // LIBPX_EDITOR_APP_HPP
