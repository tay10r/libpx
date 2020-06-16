#include "StrokeTool.hpp"

#include "Editor.hpp"
#include "Tool.hpp"

#include <libpx.hpp>

#include <imgui.h>

namespace px {

namespace {

/// Used for drawing a single line.
class StrokeTool final : public Tool
{
  /// The size of the line being drawn.
  int pixelSize = 1;
  /// The color of the line being drawn.
  float color[4] { 0, 0, 0, 1 };
  /// The current line being drawn.
  Line* line = nullptr;
  /// The last recorded cursor position.
  unsigned pos[2] { 0, 0 };
public:
  /// Constructs a new instance of the stroke tool.
  ///
  /// @param e A pointer to the editor instance that the tool is for.
  StrokeTool(Editor* e) : Tool(e) {}
  /// Handles mouse movement.
  void mouseMotion(unsigned x, unsigned y) override
  {
    pos[0] = x;
    pos[1] = y;

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

    line = addLine(getDocument());

    setPixelSize(line, pixelSize);

    setColor(line, color[0], color[1], color[2]);

    addPoint(line, pos[0], pos[1]);
    addPoint(line, pos[0], pos[1]);
  }
  /// Just a stub.
  void rightClick(bool)override { }
  /// Renders the tool properties.
  void renderProperties() override
  {
    ImGui::SliderInt("Pixel Size", &pixelSize, 1, 8, "%d");

    ImGui::ColorPicker4("Color", color);
  }
};

} // namespace

Tool* createStrokeTool(Editor* e)
{
  return new StrokeTool(e);
}

} // namespace px
