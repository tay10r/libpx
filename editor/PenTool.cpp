#include "PenTool.hpp"

namespace px {

void PenTool::onBegin(const MouseButtonEvent&, int docX, int docY)
{
  snapshotDocument();

  line = addLine(getDocument(), requireCurrentLayer());

  addPoint(line, docX, docY);
  setColor(line, getPrimaryColor());
  setBlendMode(line, getBlendMode());
  setPixelSize(line, getPixelSize());
}

void PenTool::onDrag(const MouseMotionEvent&, int docX, int docY)
{
  addPoint(line, docX, docY);
  dissolvePoints(line);
}

void PenTool::onEnd(int, int)
{
  dissolvePoints(line);
  line = nullptr;
}

} // namespace px
