#include "EllipseTool.hpp"

#include "DrawTool.hpp"
#include "Editor.hpp"

#include <libpx.hpp>

#include <imgui.h>

namespace px {

namespace {

/// Used for drawing an ellipse.
class EllipseTool final : public DrawTool
{
  /// A pointer to the ellipse being drawn.
  Ellipse* ellipse = nullptr;
  /// The starting position of the ellipse.
  unsigned start[2] { 0, 0 };
public:
  /// Constructs a new ellipse tool instance.
  ///
  /// @param e A pointer to the editor instance.
  EllipseTool(Editor* e, const DrawState& ds) : DrawTool(e, ds) {}
  /// If an ellipse is active then this function
  /// will resize it to fit the area that the mouse
  /// is dragged over.
  void mouseMotion(unsigned x, unsigned y) override
  {
    if (!ellipse) {
      return;
    }

    resizeRect(ellipse, int(start[0]), int(start[1]), int(x), int(y));
  }
  // If clicked this function will create a new ellipse.
  // If released then the function will no longer modify
  // the active ellipse.
  void leftClick(bool state) override
  {
    if (!state) {
      ellipse = nullptr;
      return;
    }

    const auto* pos = getCursor();
    const auto* color = getPrimaryColor();

    start[0] = pos[0];
    start[1] = pos[1];

    ellipse = addEllipse(getDocument());

    setColor(ellipse, color[0], color[1], color[2]);

    setPixelSize(ellipse, getPixelSize());
  }
};

} // namespace

DrawTool* createEllipseTool(Editor* editor, const DrawState& ds)
{
  return new EllipseTool(editor, ds);
}

} // namespace px
