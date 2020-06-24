#ifndef LIBPX_EDITOR_OPEN_ERROR_STATE_HPP
#define LIBPX_EDITOR_OPEN_ERROR_STATE_HPP

#include "AppState.hpp"

namespace px {

struct ErrorList;

/// This state takes over when a document
/// failed to open.
class OpenErrorState final : public AppState
{
  /// A copy of the errno variable at the point of failure.
  int errnoCopy = 0;
  /// A list of syntax errors, which may be null.
  ErrorList* errList = nullptr;
  /// Whether or not the window should be open.
  bool openFlag = true;
public:
  /// Constructs a new instance of the app state.
  ///
  /// @param app A pointer to the host app.
  /// @param errnoCopy_ The value of errno at the point of failure.
  /// @param errList_ A list of possible syntax errors.
  OpenErrorState(App* app, int errnoCopy_, ErrorList* errList_)
    : AppState(app),
      errnoCopy(errnoCopy_),
      errList(errList_) {}
  /// Releases the memory allocated by the error list.
  ~OpenErrorState();
  /// Renders a frame.
  void frame() override;
};

} // namespace px

#endif // LIBPX_EDITOR_OPEN_ERROR_STATE_HPP
