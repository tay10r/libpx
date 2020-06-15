#ifndef LIBPX_EDITOR_PEN_TOOL_HPP
#define LIBPX_EDITOR_PEN_TOOL_HPP

namespace px {

class Editor;
class Tool;

/// Creates a new instance of the pen tool.
///
/// @param editor A pointer to the editor that
/// the tool is being created for.
///
/// @return A new pen tool instance.
Tool* createPenTool(Editor* editor);

} // namespace px

#endif // LIBPX_EDITOR_PEN_TOOL_HPP
