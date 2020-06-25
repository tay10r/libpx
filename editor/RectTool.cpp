#include "RectTool.hpp"

#include "DrawTool.hpp"
#include "DrawState.hpp"

namespace px {

void RectTool::onBegin(const MouseButtonEvent&, int docX, int docY)
{
  snapshotDocument();

  startPos[0] = docX;
  startPos[1] = docY;

  rect = addQuad(getDocument(), requireCurrentLayer());

  setColor(rect, getPrimaryColor());
  setBlendMode(rect, getBlendMode());
  setPixelSize(rect, getPixelSize());
}

void RectTool::onDrag(const MouseMotionEvent&, int docX, int docY)
{
  int xi = startPos[0];
  int yi = startPos[1];

  int xf = docX;
  int yf = docY;

  setPoint(rect, 0, xi, yi);
  setPoint(rect, 1, xf, yi);
  setPoint(rect, 2, xf, yf);
  setPoint(rect, 3, xi, yf);
}

void RectTool::onEnd(int, int)
{
  rect = nullptr;
  stashDocument();
}

} // namespace px
