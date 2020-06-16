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
public:
  /// Constructs a new instance of the stroke tool.
  ///
  /// @param e A pointer to the editor instance that the tool is for.
  StrokeTool(Editor* e) : Tool(e) {}
  /// Handles mouse movement.
  void mouseMotion(unsigned x, unsigned y) override
  {
    (void)x;
    (void)y;
  }
  void leftClick(bool) override
  {
  }
  void rightClick(bool)override
  {
  }
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
