#include "ColorPickerTool.hpp"

#include "Editor.hpp"
#include "DrawTool.hpp"

#include <libpx.hpp>

namespace px {

namespace {

/// Used for getting the color on a certain point
/// of the rendered image.
class ColorPickerTool final : public DrawTool
{
public:
  /// Constructs a new color picker tool instance.
  ///
  /// @param e A pointer to the editor instance.
  ColorPickerTool(Editor* e, const DrawState& ds) : DrawTool(e, ds) {}
  /// Gets the color on the image at the current cursor location.
  void leftClick(bool state) override
  {
    if (!state) {
      return;
    }

    const auto* pos = getCursor();

    if ((pos[0] < 0) || (pos[1] < 0)) {
      return;
    }

    auto x = std::size_t(pos[0]);
    auto y = std::size_t(pos[1]);

    const auto* img = getImage();

    getColor(img, x, y, getPrimaryColor());
  }
};

} // namespace

DrawTool* createColorPickerTool(Editor* editor, const DrawState& ds)
{
  return new ColorPickerTool(editor, ds);
}

} // namespace px
