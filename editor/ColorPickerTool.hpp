#ifndef LIBPX_EDITOR_COLOR_PICKER_TOOL_HPP
#define LIBPX_EDITOR_COLOR_PICKER_TOOL_HPP

#include "DrawTool.hpp"

namespace px {

class ColorPickerTool final : public DrawTool
{
public:
  using DrawTool::DrawTool;
protected:
  void onBegin(const MouseButtonEvent&, int, int) override;
  void onDrag(const MouseMotionEvent&, int, int) override;
  void pick(int docX, int docY);
};

} // namespace px

#endif // LIBPX_EDITOR_COLOR_PICKER_TOOL_HPP
