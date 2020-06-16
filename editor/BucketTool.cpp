#include "BucketTool.hpp"

#include "Editor.hpp"
#include "Tool.hpp"

#include <libpx.hpp>

#include <imgui.h>

namespace px {

namespace {

/// Used for filling the area of an image
/// with a certain color.
class BucketTool final : public Tool
{
  /// The color being drawn with.
  float color[4] { 0, 0, 0, 1 };
  /// The X position to apply the bucket tool.
  unsigned posX = 0;
  /// The Y position to apply the bucket tool.
  unsigned posY = 0;
public:
  /// Constructs a new bucket tool instance.
  ///
  /// @param e A pointer to the editor instance.
  BucketTool(Editor* e) : Tool(e) {}
  /// Tracks the position for
  /// the next fill operation.
  void mouseMotion(unsigned x, unsigned y) override
  {
    posX = x;
    posY = y;
  }
  /// Handles a left click state change.
  /// If the button is being pressed, a new path is started.
  void leftClick(bool state) override
  {
    if (!state) {
      return;
    }

    Fill* fill = addFill(getDocument());
    if (fill) {

      setFillOrigin(fill, posX, posY);

      setColor(fill, color[0], color[1], color[2]);
    }
  }
  /// Just a stub.
  void rightClick(bool) override { }
  /// Renders the tool properties.
  void renderProperties() override
  {
    ImGui::ColorPicker4("Color", color);
  }
};

} // namespace

Tool* createBucketTool(Editor* editor)
{
  return new BucketTool(editor);
}

} // namespace px
