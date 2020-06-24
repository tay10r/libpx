#ifndef LIBPX_EDITOR_BROWSE_DOCUMENTS_STATE_HPP
#define LIBPX_EDITOR_BROWSE_DOCUMENTS_STATE_HPP

#include "AppState.hpp"

namespace px {

struct Document;
struct Image;

/// This state is used for choosing a document to open.
class BrowseDocumentsState : public AppState
{
public:
  using AppState::AppState;
  /// Constructs a new app state for browsing documents.
  ///
  /// @param app A pointer to the application hosting the state.
  static BrowseDocumentsState* init(App* app);
  /// Just a stub.
  virtual ~BrowseDocumentsState() {}
};

} // namespace px

#endif // LIBPX_EDITOR_BROWSE_DOCUMENTS_STATE_HPP
