#include "PenTool.hpp"

#include "DrawTool.hpp"
#include "DrawMode.hpp"

#include <libpx.hpp>

#include <imgui.h>

namespace px {

namespace {

/// Used for erasing contents from the image.
class EraserTool final : public DrawTool
{
  /// The line used to erase the image content.
  Line* line = nullptr;
public:
  /// Constructs a new eraser tool instance.
  EraserTool(DrawMode* d) : DrawTool(d) {}
  /// Handles mouse motion.
  /// If the left mouse button is clicked,
  /// the point passed to this function is
  /// added to the current line.
  void mouseMotion(unsigned x, unsigned y) override
  {
    if (line) {
      addPoint(line, x, y);
    }
  }
  /// Handles a left click state change.
  /// If the button is being pressed, a new path is started.
  void leftClick(bool state) override
  {
    if (!state) {
      // Mouse dragging causes a lot of redundant
      // points. Calling this function simplifies the
      // line and removes the redundancies.
      dissolvePoints(line);
      line = nullptr;
      return;
    }

    snapshotDoc();

    const auto* pos = getCursor();

    line = addLine(getDocument(), requireCurrentLayer());

    setPixelSize(line, getPixelSize());
    addPoint(line, pos[0], pos[1]);
    setColor(line, 1.0f, 1.0f, 1.0f);
    setBlendMode(line, BlendMode::Subtract);
  }
};

} // namespace

DrawTool* createEraserTool(DrawMode* d)
{
  return new EraserTool(d);
}

} // namespace px
