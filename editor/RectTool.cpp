#include "RectTool.hpp"

#include "DrawTool.hpp"
#include "Editor.hpp"

#include <libpx.hpp>

#include <imgui.h>

namespace px {

namespace {

/// Used for drawing a rectangle.
class RectTool final : public DrawTool
{
  /// The starting point of the quad.
  unsigned startPos[2] { 0, 0 };
  /// If not null then points to the
  /// quadrilateral being drawn.
  Quad* rect = nullptr;
public:
  /// Constructs a new instance of the stroke tool.
  ///
  /// @param e A pointer to the editor instance that the tool is for.
  RectTool(Editor* e, const DrawState& ds) : DrawTool(e, ds) {}
  /// Handles mouse movement.
  void mouseMotion(unsigned x, unsigned y) override
  {
    if (!rect) {
      return;
    }

    int xi = int(startPos[0]);
    int yi = int(startPos[1]);

    int xf = int(x);
    int yf = int(y);

    setPoint(rect, 0, xi, yi);
    setPoint(rect, 1, xf, yi);
    setPoint(rect, 2, xf, yf);
    setPoint(rect, 3, xi, yf);
  }
  /// Creates a new line if the state is true.
  /// Finishes the current line if the state is false.
  void leftClick(bool state) override
  {
    if (!state) {
      rect = nullptr;
      return;
    }

    snapshotDoc();

    const auto* pos = getCursor();
    const auto* color = getPrimaryColor();

    rect = addQuad(getDocument());

    setPoint(rect, 0, pos[0], pos[1]);
    setPixelSize(rect, getPixelSize());
    setColor(rect, color[0], color[1], color[2]);

    startPos[0] = pos[0];
    startPos[1] = pos[1];
  }
};

} // namespace

DrawTool* createRectTool(Editor* e, const DrawState& ds)
{
  return new RectTool(e, ds);
}

} // namespace px
