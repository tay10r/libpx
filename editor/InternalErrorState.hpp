#ifndef LIBPX_EDITOR_INTERNAL_ERROR_STATE_HPP
#define LIBPX_EDITOR_INTERNAL_ERROR_STATE_HPP

#include "AppState.hpp"

namespace px {

/// This state happens when an unexpected
/// error occurs that prevents the application
/// from running. It presents the user with a
/// dialog and encourages them to open an issue.
class InternalErrorState final : public AppState
{
public:
  using AppState::AppState;
  /// Renders the app state.
  void frame() override;
};

} // namespace px

#endif // LIBPX_EDITOR_INTERNAL_ERROR_STATE_HPP
