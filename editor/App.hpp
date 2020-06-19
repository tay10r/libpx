#ifndef LIBPX_EDITOR_APP_HPP
#define LIBPX_EDITOR_APP_HPP

namespace px {

class Platform;

/// This is the interface for the application.
/// The @ref Platform and @ref App class most communicate
/// with each other via their interfaces.
class App
{
public:
  /// Creates a new instance of the app.
  ///
  /// @param platform A pointer to the platform
  /// that is hosting the app instance.
  ///
  /// @return A pointer to the app.
  static App* init(Platform* platform);
  /// Just a stub.
  virtual ~App() {}
  /// Parses arguments from the command line.
  ///
  /// @param argc The number of arguments to parse.
  /// @param argv The argument array.
  ///
  /// @return True on success, false on failure.
  virtual bool parseArgs(int argc, char** argv) = 0;
  /// Called to render a frame.
  ///
  /// @return True on success, false on failure.
  virtual bool frame() = 0;
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
