#ifndef LIBPX_EDITOR_STROKE_TOOL_HPP
#define LIBPX_EDITOR_STROKE_TOOL_HPP

#include "DrawTool.hpp"

namespace px {

class DrawTool;
class DrawState;

class StrokeTool final : public DrawTool
{
  Line* line = nullptr;
public:
  using DrawTool::DrawTool;
protected:
  void onBegin(const MouseButtonEvent&, int, int) override;
  void onDrag(const MouseMotionEvent&, int, int) override;
  void onEnd(int, int) override;
};

} // namespace px

#endif // LIBPX_EDITOR_STROKE_TOOL_HPP
