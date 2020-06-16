#include "DrawMode.hpp"

#include "Editor.hpp"
#include "Mode.hpp"
#include "Tool.hpp"

#include "BucketTool.hpp"
#include "PenTool.hpp"
#include "RectTool.hpp"
#include "StrokeTool.hpp"

#include <libpx.hpp>

#include <imgui.h>

#include <memory>

namespace px {

namespace {

/// This mode is made for drawing operations.
class DrawMode final : public Mode
{
  /// A pointer to the editor hosting the mode.
  Editor* editor = nullptr;
  /// The currently selected tool.
  std::unique_ptr<Tool> currentTool;
  /// The index of the currently selected tool.
  int toolIndex = 0;
  /// Whether or not the document size is locked.
  bool sizeLock = true;
  /// The last known cursor position.
  unsigned cursor[2] { 0, 0 };
public:
  /// Constructs a new instance of the draw mode.
  ///
  /// @param e A pointer to the editor that the draw mode is for.
  DrawMode(Editor* e) : editor(e)
  {
    currentTool = std::unique_ptr<Tool>(createPenTool(e));
  }
  /// Passes the left click state change
  /// to the currently active tool.
  void leftClick(bool state) override
  {
    if (currentTool) {
      currentTool->leftClick(state);
    }
  }
  /// Passes the mouse motion event
  /// to the currently selected tool.
  void mouseMotion(unsigned x, unsigned y) override
  {
    cursor[0] = x;
    cursor[1] = y;

    if (currentTool) {
      currentTool->mouseMotion(x, y);
    }
  }
  /// Renders the mode widgets.
  void render() override
  {
    renderDrawPanel();
  }
  /// Passes the right click state change
  /// to the currently active tool.
  void rightClick(bool state) override
  {
    if (currentTool) {
      currentTool->rightClick(state);
    }
  }
protected:
  /// Renders the drawing panel controls.
  void renderDrawPanel()
  {
    ImGui::Begin("Draw Panel", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

    if (ImGui::CollapsingHeader("Available Tools")) {

      auto hit = false;

      hit |= ImGui::RadioButton(         "Pen", &toolIndex, 0);
      hit |= ImGui::RadioButton(      "Stroke", &toolIndex, 1);
      hit |= ImGui::RadioButton(      "Bucket", &toolIndex, 2);
      hit |= ImGui::RadioButton(   "Rectangle", &toolIndex, 3);
      hit |= ImGui::RadioButton(     "Ellipse", &toolIndex, 4);
      hit |= ImGui::RadioButton("Color Picker", &toolIndex, 5);

      if (hit) {
        switch (toolIndex) {
          case 0:
            currentTool.reset(createPenTool(editor));
            break;
          case 1:
            currentTool.reset(createStrokeTool(editor));
            break;
          case 2:
            currentTool.reset(createBucketTool(editor));
            break;
          case 3:
            currentTool.reset(createRectTool(editor));
            break;
          case 4:
            currentTool.reset();
            break;
          case 5:
            currentTool.reset();
            break;
        }
      }
    }

    if (ImGui::CollapsingHeader("Tool Properties", false)) {
      if (currentTool) {
        currentTool->renderProperties();
      }
    }

    if (ImGui::CollapsingHeader("Document Properties", false)) {

      auto* doc = editor->getDocument();
      int w = getDocWidth(doc);
      int h = getDocHeight(doc);

      ImGui::InputInt( "Width", &w, 2, 32);
      ImGui::InputInt("Height", &h, 2, 32);
      ImGui::Checkbox("Size Lock", &sizeLock);

      if (!sizeLock) {
        resizeDoc(doc, w, h);
      }
    }

    ImGui::Text("Cursor: (%u, %u)", cursor[0], cursor[1]);

    ImGui::End();
  }
};

} // namespace

Mode* createDrawMode(Editor* editor)
{
  return new DrawMode(editor);
}

} // namespace px
