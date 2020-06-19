#include "App.hpp"

#include "Platform.hpp"

#include <imgui.h>

namespace px {

namespace {

/// Implements the interface to the application.
class AppImpl final : public App
{
  /// A pointer to the platform that is hosting the app.
  Platform* platform = nullptr;
public:
  /// Constructs a new app instance.
  AppImpl(Platform* p) : platform(p) {}
  /// Releases memory allocated by the app.
  ~AppImpl() {}
  /// Checks for any non-options that may be interpreted
  /// as a document to be opened.
  bool parseArgs(int, char**) override
  {
    return true;
  }
  /// Renders a frame of the application.
  bool frame() override
  {
    try {
      uncheckedFrame();
    } catch (...) {
      return false;
    }

    return true;
  }
protected:
  /// This function renders a frame without checking for
  /// exceptions.
  ///
  /// Exceptions are checked by the calling function.
  void uncheckedFrame()
  {
    platform->clear(1, 1, 1, 1);
  }
};

} // namesapce

App* App::init(Platform* platform) { return new AppImpl(platform); }

} // namespace px
