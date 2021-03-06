#ifndef LIBPX_EDITOR_RECT_TOOL_HPP
#define LIBPX_EDITOR_RECT_TOOL_HPP

#include "DrawTool.hpp"

namespace px {

class DrawTool;
class DrawState;

class RectTool final : public DrawTool
{
  Quad* rect = nullptr;
  int startPos[2] { 0, 0 };
public:
  using DrawTool::DrawTool;
protected:
  void onBegin(const MouseButtonEvent&, int, int) override;
  void onDrag(const MouseMotionEvent&, int, int) override;
  void onEnd(int, int) override;
};

} // namespace px
#endif // LIBPX_EDITOR_RECT_TOOL_HPP
