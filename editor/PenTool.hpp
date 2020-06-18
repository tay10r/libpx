#ifndef LIBPX_EDITOR_PEN_TOOL_HPP
#define LIBPX_EDITOR_PEN_TOOL_HPP

namespace px {

class DrawTool;
class DrawMode;

/// Creates a new instance of the pen tool.
///
/// @return A new pen tool instance.
DrawTool* createPenTool(DrawMode* d);

} // namespace px

#endif // LIBPX_EDITOR_PEN_TOOL_HPP
