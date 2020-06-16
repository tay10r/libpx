#ifndef LIBPX_EDITOR_RECT_TOOL_HPP
#define LIBPX_EDITOR_RECT_TOOL_HPP

namespace px {

struct DrawState;

class DrawTool;
class Editor;

/// Creates a new instance of the rectangle tool.
///
/// @param editor A pointer to the editor that the tool is for.
///
/// @return a new rectangle tool instance.
DrawTool* createRectTool(Editor* editor, const DrawState& ds);

} // namespace px

#endif // LIBPX_EDITOR_RECT_TOOL_HPP
