#ifndef LIBPX_EDITOR_STROKE_TOOL_HPP
#define LIBPX_EDITOR_STROKE_TOOL_HPP

namespace px {

struct DrawState;

class DrawTool;
class Editor;

/// Creates a new instance of the stroke tool.
///
/// @param editor A pointer to the editor that
/// the tool is being created for.
///
/// @return A new stroke tool instance.
DrawTool* createStrokeTool(Editor* editor, const DrawState& ds);

} // namespace px

#endif // LIBPX_EDITOR_STROKE_TOOL_HPP
