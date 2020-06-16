#ifndef LIBPX_EDITOR_STROKE_TOOL_HPP
#define LIBPX_EDITOR_STROKE_TOOL_HPP

namespace px {

class Editor;
class Tool;

/// Creates a new instance of the stroke tool.
///
/// @param editor A pointer to the editor that
/// the tool is being created for.
///
/// @return A new stroke tool instance.
Tool* createStrokeTool(Editor* editor);

} // namespace px

#endif // LIBPX_EDITOR_STROKE_TOOL_HPP
