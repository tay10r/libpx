#ifndef LIBPX_EDITOR_COLOR_PICKER_TOOL_HPP
#define LIBPX_EDITOR_COLOR_PICKER_TOOL_HPP

namespace px {

struct DrawState;

class DrawTool;
class Editor;

/// Creates a new instance of the color picker tool.
///
/// @param editor A pointer to the editor that
/// the tool is being created for.
///
/// @return A new color picker tool instance.
DrawTool* createColorPickerTool(Editor* editor, const DrawState& ds);

} // namespace px

#endif // LIBPX_EDITOR_COLOR_PICKER_TOOL_HPP
