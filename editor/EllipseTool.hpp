#ifndef LIBPX_EDITOR_ELLIPSE_TOOL_HPP
#define LIBPX_EDITOR_ELLIPSE_TOOL_HPP

namespace px {

class DrawTool;
class DrawMode;

/// Creates a new instance of the ellipse tool.
///
/// @return A new ellipse tool instance.
DrawTool* createEllipseTool(DrawMode* d);

} // namespace px

#endif // LIBPX_EDITOR_ELLIPSE_TOOL_HPP
