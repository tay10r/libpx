#ifndef LIBPX_EDITOR_RECT_TOOL_HPP
#define LIBPX_EDITOR_RECT_TOOL_HPP

namespace px {

class DrawTool;
class DrawMode;

/// Creates a new instance of the rectangle tool.
///
/// @return a new rectangle tool instance.
DrawTool* createRectTool(DrawMode* d);

} // namespace px

#endif // LIBPX_EDITOR_RECT_TOOL_HPP
