#include "EllipseTool.hpp"

#include "DrawState.hpp"

namespace px {

void EllipseTool::onBegin(const MouseButtonEvent&, int docX, int docY)
{
  snapshotDocument();

  startPos[0] = docX;
  startPos[1] = docY;

  ellipse = addEllipse(getDocument(), requireCurrentLayer());

  setColor(ellipse, getPrimaryColor());
  setBlendMode(ellipse, getBlendMode());
  setPixelSize(ellipse, getPixelSize());
}

void EllipseTool::onDrag(const MouseMotionEvent&, int docX, int docY)
{
  resizeRect(ellipse, startPos[0], startPos[1], docX, docY);
}

void EllipseTool::onEnd(int, int)
{
  ellipse = nullptr;
  stashDocument();
}

} // namespace px
