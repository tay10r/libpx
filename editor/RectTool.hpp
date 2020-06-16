#ifndef LIBPX_EDITOR_RECT_TOOL_HPP
#define LIBPX_EDITOR_RECT_TOOL_HPP

namespace px {

class Editor;
class Tool;

/// Creates a new instance of the rectangle tool.
///
/// @param editor A pointer to the editor that the tool is for.
///
/// @return a new rectangle tool instance.
Tool* createRectTool(Editor* editor);

} // namespace px

#endif // LIBPX_EDITOR_RECT_TOOL_HPP
