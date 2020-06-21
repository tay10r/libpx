#include "ColorPickerTool.hpp"

#include "DrawState.hpp"

namespace px {

void ColorPickerTool::onBegin(const MouseButtonEvent&, int docX, int docY)
{
  pick(docX, docY);
}

void ColorPickerTool::onDrag(const MouseMotionEvent&, int docX, int docY)
{
  pick(docX, docY);
}

void ColorPickerTool::pick(int docX, int docY)
{
  if ((docX < 0) || (docY < 0)) {
    return;
  }

  auto x = std::size_t(docX);
  auto y = std::size_t(docY);

  const auto* img = getImage();
  auto w = getImageWidth(img);
  auto h = getImageHeight(img);
  if ((x >= w) || (y >= h)) {
    return;
  }

  getColor(img, x, y, getPrimaryColor());
}

} // namespace px
