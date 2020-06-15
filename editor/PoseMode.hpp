#ifndef LIBPX_EDITOR_POSE_MODE_HPP
#define LIBPX_EDITOR_POSE_MODE_HPP

namespace px {

class Editor;
class Mode;

/// Creates a new pose mode instance.
///
/// @param editor The editor that the mode is
/// being created for.
///
/// @return A new pose mode instance.
Mode* createPoseMode(Editor* editor);

} // namespace px

#endif // LIBPX_EDITOR_POSE_MODE_HPP
