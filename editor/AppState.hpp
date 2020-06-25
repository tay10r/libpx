#ifndef LIBPX_EDITOR_APP_STATE_HPP
#define LIBPX_EDITOR_APP_STATE_HPP

namespace px {

struct Document;
struct KeyEvent;
struct MouseButtonEvent;
struct MouseMotionEvent;

class App;
class Log;
class MenuBar;
class Platform;

/// This is the base class for a state
/// that the app can be in.
class AppState
{
  /// A pointer to the app instance.
  App* app = nullptr;
  /// Whether or not the application state should be closed.
  bool closeFlag = false;
public:
  /// Constructs a new base app state.
  ///
  /// @param app_ A pointer to the app hosting the state.
  AppState(App* app_) : app(app_) {}
  /// Just a stub.
  virtual ~AppState() {}
  /// Called on every frame to render the state.
  virtual void frame() = 0;
  /// Gets a pointer to the app.
  ///
  /// @return A pointer to the app, for non-const access.
  inline App* getApp() noexcept { return app; }
  /// Gets a pointer to the app.
  ///
  /// @return A pointer to the app, for const access.
  inline const App* getApp() const noexcept { return app; }
  /// Creates a snapshot of the document.
  /// This is used primarily for undo/redo purposes.
  void snapshotDocument();
  /// Gets a pointer to the document.
  ///
  /// @return A pointer to the document, for non-const access.
  Document* getDocument() noexcept;
  /// Gets a pointer to the document.
  ///
  /// @return A pointer to the document, for const access.
  const Document* getDocument() const noexcept;
  /// Gets a pointer to the menu bar.
  ///
  /// @return A pointer to the menu bar, for non-const access.
  MenuBar* getMenuBar() noexcept;
  /// Gets a pointer to the menu bar.
  ///
  /// @return A pointer to the menu bar, for const access.
  const MenuBar* getMenuBar() const noexcept;
  /// Gets a pointer to the log.
  ///
  /// @return A pointer to the log, for non-const access.
  Log* getLog() noexcept;
  /// Gets a pointer to the log.
  ///
  /// @return A pointer to the log, for const access.
  const Log* getLog() const noexcept;
  /// Gets a pointer to the platform.
  ///
  /// @return A pointer to the platform.
  Platform* getPlatform() noexcept;
  /// Handles a keyboard event.
  virtual void key(const KeyEvent&) {}
  /// Handles a mouse button event.
  virtual void mouseButton(const MouseButtonEvent&) {}
  /// Handles a mouse motion event.
  virtual void mouseMotion(const MouseMotionEvent&) {}
  /// Whether or not the state should be closed on the next iteration.
  inline bool shouldClose() const noexcept { return closeFlag; }
  /// Synchronizes the data shown by application
  /// states with the data that's actually in the document.
  /// This is called, for example, at each undo or redo.
  ///
  /// @param doc The document to synchronize the data with.
  virtual void syncDocument(const Document* doc)
  {
    (void)doc;
  }
protected:
  /// Queues the state to be removed.
  void queueClose()
  {
    closeFlag = true;
  }
};

} // namespace px

#endif // LIBPX_EDITOR_APP_STATE_HPP
