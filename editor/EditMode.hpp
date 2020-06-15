#ifndef LIBPX_EDITOR_EDIT_MODE_HPP
#define LIBPX_EDITOR_EDIT_MODE_HPP

namespace px {

class Editor;
class Mode;

/// Creates a new edit mode instance.
///
/// @param editor The editor that the mode is
/// being created for.
///
/// @return A new edit mode instance.
Mode* createEditMode(Editor* editor);

} // namespace px

#endif // LIBPX_EDITOR_EDIT_MODE_HPP
