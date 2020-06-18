#include "StrokeTool.hpp"

#include "DrawTool.hpp"
#include "DrawMode.hpp"

#include <libpx.hpp>

namespace px {

namespace {

/// Used for drawing a single line.
class StrokeTool final : public DrawTool
{
  /// The current line being drawn.
  Line* line = nullptr;
public:
  /// Constructs a new instance of the stroke tool.
  StrokeTool(DrawMode* d) : DrawTool(d) {}
  /// Handles mouse movement.
  void mouseMotion(unsigned x, unsigned y) override
  {
    if (!line) {
      return;
    }

    setPoint(line, 1, x, y);
  }
  /// Creates a new line if the state is true.
  /// Finishes the current line if the state is false.
  void leftClick(bool state) override
  {
    if (!state) {
      line = nullptr;
      return;
    }

    snapshotDoc();

    const auto* pos = getCursor();
    const auto* color = getPrimaryColor();

    line = addLine(getDocument(), requireCurrentLayer());

    setPixelSize(line, getPixelSize());
    setColor(line, color[0], color[1], color[2], color[3]);
    setBlendMode(line, getBlendMode());
    addPoint(line, pos[0], pos[1]);
    addPoint(line, pos[0], pos[1]);
  }
};

} // namespace

DrawTool* createStrokeTool(DrawMode* d)
{
  return new StrokeTool(d);
}

} // namespace px
