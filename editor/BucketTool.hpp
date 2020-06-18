#ifndef LIBPX_EDITOR_BUCKET_TOOL_HPP
#define LIBPX_EDITOR_BUCKET_TOOL_HPP

namespace px {

class DrawTool;
class DrawMode;

/// Creates a new instance of the bucket tool.
///
/// @return A new bucket tool instance.
DrawTool* createBucketTool(DrawMode* drawMode);

} // namespace px

#endif // LIBPX_EDITOR_BUCKET_TOOL_HPP
