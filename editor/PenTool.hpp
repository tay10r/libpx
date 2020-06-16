#ifndef LIBPX_EDITOR_PEN_TOOL_HPP
#define LIBPX_EDITOR_PEN_TOOL_HPP

namespace px {

struct DrawState;

class DrawTool;
class Editor;

/// Creates a new instance of the pen tool.
///
/// @param editor A pointer to the editor that
/// the tool is being created for.
///
/// @return A new pen tool instance.
DrawTool* createPenTool(Editor* editor, const DrawState& ds);

} // namespace px

#endif // LIBPX_EDITOR_PEN_TOOL_HPP
