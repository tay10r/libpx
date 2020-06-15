#ifndef LIBPX_EDITOR_BUCKET_TOOL_HPP
#define LIBPX_EDITOR_BUCKET_TOOL_HPP

namespace px {

class Editor;
class Tool;

/// Creates a new instance of the bucket tool.
///
/// @param editor A pointer to the editor that
/// the tool is being created for.
///
/// @return A new bucket tool instance.
Tool* createBucketTool(Editor* editor);

} // namespace px

#endif // LIBPX_EDITOR_BUCKET_TOOL_HPP
