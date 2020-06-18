#ifndef LIBPX_EDITOR_COLOR_PICKER_TOOL_HPP
#define LIBPX_EDITOR_COLOR_PICKER_TOOL_HPP

namespace px {

class DrawTool;
class DrawMode;

/// Creates a new instance of the color picker tool.
///
/// @return A new color picker tool instance.
DrawTool* createColorPickerTool(DrawMode* d);

} // namespace px

#endif // LIBPX_EDITOR_COLOR_PICKER_TOOL_HPP
