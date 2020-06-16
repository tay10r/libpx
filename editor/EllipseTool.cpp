#include "EllipseTool.hpp"

#include "Editor.hpp"
#include "Tool.hpp"

#include <libpx.hpp>

#include <imgui.h>

namespace px {

namespace {

/// Used for drawing an ellipse.
class EllipseTool final : public Tool
{
  /// The color being drawn with.
  float color[3] { 0, 0, 0 };
  /// The pixel size to assign the ellipse.
  int pixelSize = 1;
  /// A pointer to the ellipse being drawn.
  Ellipse* ellipse = nullptr;
  /// The starting position of the ellipse.
  unsigned start[2] { 0, 0 };
  /// The last known position of the ellipse.
  unsigned pos[2] { 0, 0 };
public:
  /// Constructs a new ellipse tool instance.
  ///
  /// @param e A pointer to the editor instance.
  EllipseTool(Editor* e) : Tool(e) {}
  /// If an ellipse is active then this function
  /// will resize it to fit the area that the mouse
  /// is dragged over.
  void mouseMotion(unsigned x, unsigned y) override
  {
    pos[0] = x;
    pos[1] = y;

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

    start[0] = pos[0];
    start[1] = pos[1];

    ellipse = addEllipse(getDocument());

    setColor(ellipse, color[0], color[1], color[2]);

    setPixelSize(ellipse, pixelSize);
  }
  /// Just a stub.
  void rightClick(bool) override { }
  /// Renders the tool properties.
  void renderProperties() override
  {
    ImGui::SliderInt("Pixel Size", &pixelSize, 1, 8, "%d");

    ImGui::ColorPicker3("Color", color);
  }
};

} // namespace

Tool* createEllipseTool(Editor* editor)
{
  return new EllipseTool(editor);
}

} // namespace px
