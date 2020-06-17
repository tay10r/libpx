#ifndef LIBPX_EDITOR_SAVE_DIALOG_HPP
#define LIBPX_EDITOR_SAVE_DIALOG_HPP

namespace px {

class Dialog;

/// Creates a new save dialog instance.
/// This assists the user in saving the document to a certain path.
///
/// @return A pointer to a new save dialog instance.
Dialog* createSaveDialog();

} // namespace px

#endif // LIBPX_EDITOR_SAVE_DIALOG_HPP
