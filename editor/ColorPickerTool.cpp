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
  ColorPickerTool(DrawMode* d) : DrawTool(d) {}
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

DrawTool* createColorPickerTool(DrawMode* d)
{
  return new ColorPickerTool(d);
}

} // namespace px
