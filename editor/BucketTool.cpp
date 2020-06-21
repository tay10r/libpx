#include "BucketTool.hpp"

namespace px {

void BucketTool::onBegin(const MouseButtonEvent&, int docX, int docY)
{
  snapshotDocument();

  const auto* color = getPrimaryColor();

  Fill* fill = addFill(getDocument(), requireCurrentLayer());
  setFillOrigin(fill, docX, docY);
  setColor(fill, color[0], color[1], color[2], color[3]);
  setBlendMode(fill, getBlendMode());
}

} // namespace px
