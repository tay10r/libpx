#ifndef LIBPX_EDITOR_ELLIPSE_TOOL_HPP
#define LIBPX_EDITOR_ELLIPSE_TOOL_HPP

namespace px {

class Editor;
class Tool;

/// Creates a new instance of the ellipse tool.
///
/// @param editor A pointer to the editor that
/// the tool is being created for.
///
/// @return A new ellipse tool instance.
Tool* createEllipseTool(Editor* editor);

} // namespace px

#endif // LIBPX_EDITOR_ELLIPSE_TOOL_HPP
