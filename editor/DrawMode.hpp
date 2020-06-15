#ifndef LIBPX_EDITOR_DRAW_MODE_HPP
#define LIBPX_EDITOR_DRAW_MODE_HPP

namespace px {

class Editor;
class Mode;

/// Creates a new draw mode instance.
///
/// @param editor A pointer to the editor interface.
///
/// @return A pointer to the draw mode.
Mode* createDrawMode(Editor* editor);

} // namespace px

#endif // LIBPX_EDITOR_DRAW_MODE_HPP
