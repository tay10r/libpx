#include "PenTool.hpp"

#include "DrawTool.hpp"
#include "Editor.hpp"

#include <libpx.hpp>

#include <imgui.h>

namespace px {

namespace {

/// Used for freestyle drawing.
class PenTool final : public DrawTool
{
  /// The current line being drawn.
  Line* line = nullptr;
public:
  /// Constructs a new pen tool instance.
  ///
  /// @param e A pointer to the editor instance.
  PenTool(Editor* e, const DrawState& ds) : DrawTool(e, ds) {}
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
      line = nullptr;
      return;
    }

    const auto* color = getPrimaryColor();
    const auto* pos = getCursor();

    line = addLine(getDocument());

    setPixelSize(line, getPixelSize());
    addPoint(line, pos[0], pos[1]);
    setColor(line, color[0], color[1], color[2]);
  }
};

} // namespace

DrawTool* createPenTool(Editor* editor, const DrawState& ds)
{
  return new PenTool(editor, ds);
}

} // namespace px
