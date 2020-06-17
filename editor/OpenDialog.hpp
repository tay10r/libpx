#ifndef LIBPX_EDITOR_OPEN_DIALOG_HPP
#define LIBPX_EDITOR_OPEN_DIALOG_HPP

namespace px {

class Dialog;

/// Creates a dialog for opening an existing document.
///
/// @return a new dialog instance.
Dialog* createOpenDialog();

} // namepsace px

#endif // LIBPX_EDITOR_OPEN_DIALOG_HPP
