#ifndef LIBPX_EDITOR_ERASER_TOOL_HPP
#define LIBPX_EDITOR_ERASER_TOOL_HPP

namespace px {

class DrawTool;
class DrawMode;

/// Creates a new instance of the eraser tool.
///
/// @return A new eraser tool instance.
DrawTool* createEraserTool(DrawMode* drawMode);

} // namespace px

#endif // LIBPX_EDITOR_ERASER_TOOL_HPP
