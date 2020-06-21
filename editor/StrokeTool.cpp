#include "StrokeTool.hpp"

#include "DrawState.hpp"

namespace px {

void StrokeTool::onBegin(const MouseButtonEvent&, int docX, int docY)
{
  snapshotDocument();

  line = addLine(getDocument(), requireCurrentLayer());

  setColor(line, getPrimaryColor());
  setBlendMode(line, getBlendMode());
  setPixelSize(line, getPixelSize());
  addPoint(line, docX, docY);
  addPoint(line, docX, docY);
}

void StrokeTool::onDrag(const MouseMotionEvent&, int docX, int docY)
{
  setPoint(line, 1, docX, docY);
}

void StrokeTool::onEnd(int, int)
{
  line = nullptr;
}

} // namespace px
