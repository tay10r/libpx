#ifndef LIBPX_EDITOR_PEN_TOOL_HPP
#define LIBPX_EDITOR_PEN_TOOL_HPP

#include "DrawTool.hpp"

namespace px {

class PenTool final : public DrawTool
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

#endif // LIBPX_EDITOR_PEN_TOOL_HPP
