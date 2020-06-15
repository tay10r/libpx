#ifndef LIBPX_EDITOR_EXPORT_DIALOG_HPP
#define LIBPX_EDITOR_EXPORT_DIALOG_HPP

namespace px {

class Dialog;
class Export;

/// Creates a new export dialog instance.
///
/// @return A pointer to a new export dialog instance.
Dialog* createExportDialog();

} // namespace px

#endif // LIBPX_EDITOR_EXPORT_DIALOG_HPP
