#include "BucketTool.hpp"

#include "Editor.hpp"
#include "DrawTool.hpp"

#include <libpx.hpp>

#include <imgui.h>

namespace px {

namespace {

/// Used for filling the area of an image
/// with a certain color.
class BucketTool final : public DrawTool
{
public:
  /// Constructs a new bucket tool instance.
  ///
  /// @param e A pointer to the editor instance.
  BucketTool(Editor* e, const DrawState& ds) : DrawTool(e, ds) {}
  /// Handles a left click state change.
  /// If the button is being pressed, a new path is started.
  void leftClick(bool state) override
  {
    if (!state) {
      return;
    }

    snapshotDoc();

    const auto* color = getPrimaryColor();
    const auto* cursor = getCursor();

    Fill* fill = addFill(getDocument());
    if (fill) {
      setFillOrigin(fill, cursor[0], cursor[1]);
      setColor(fill, color[0], color[1], color[2]);
    }
  }
  /// Just a stub.
  void rightClick(bool) override { }
};

} // namespace

DrawTool* createBucketTool(Editor* editor, const DrawState& ds)
{
  return new BucketTool(editor, ds);
}

} // namespace px
