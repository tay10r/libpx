#include "EraserTool.hpp"

namespace px {

void EraserTool::onBegin(const MouseButtonEvent&, int docX, int docY)
{
  snapshotDocument();

  line = addLine(getDocument());

  addPoint(line, docX, docY);
  setColor(line, 1, 1, 1, 1);
  setBlendMode(line, BlendMode::Subtract);
  setPixelSize(line, getPixelSize());
}

void EraserTool::onDrag(const MouseMotionEvent&, int docX, int docY)
{
  addPoint(line, docX, docY);
  dissolvePoints(line);
}

void EraserTool::onEnd(int, int)
{
  dissolvePoints(line);
  line = nullptr;
  stashDocument();
}

} // namespace px
