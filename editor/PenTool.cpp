#include "PenTool.hpp"

#include "Editor.hpp"
#include "Tool.hpp"

#include <libpx.hpp>

#include <imgui.h>

namespace px {

namespace {

/// Used for freestyle drawing.
class PenTool final : public Tool
{
  /// The size of the pixels in the lines being drawn.
  int pixelSize = 1;
  /// The color being drawn with.
  float color[4] { 0, 0, 0, 1 };
  /// The current line being drawn.
  Line* line = nullptr;
public:
  /// Constructs a new pen tool instance.
  ///
  /// @param e A pointer to the editor instance.
  PenTool(Editor* e) : Tool(e) {}
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
    if (state) {
      line = addLine(getDocument());

      setPixelSize(line, pixelSize);

      setLineColor(line, color[0],
                         color[1],
                         color[2],
                         color[3]);
    } else {
      line = nullptr;
    }
  }
  void rightClick(bool) override
  {
  }
  /// Renders the tool properties.
  void renderProperties() override
  {
    ImGui::SliderInt("Pixel Size", &pixelSize, 1, 8, "%d");

    ImGui::ColorPicker4("Color", color);
  }
};

} // namespace

Tool* createPenTool(Editor* editor)
{
  return new PenTool(editor);
}

} // namespace px
