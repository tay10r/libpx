#ifndef LIBPX_EDITOR_STROKE_TOOL_HPP
#define LIBPX_EDITOR_STROKE_TOOL_HPP

namespace px {

class DrawTool;
class DrawMode;

/// Creates a new instance of the stroke tool.
///
/// @return A new stroke tool instance.
DrawTool* createStrokeTool(DrawMode* d);

} // namespace px

#endif // LIBPX_EDITOR_STROKE_TOOL_HPP
