#include "DrawTool.hpp"

#include "App.hpp"
#include "DrawPanel.hpp"
#include "DrawState.hpp"

namespace px {

void DrawTool::snapshotDocument()
{
  drawState->snapshotDocument();
}

Document* DrawTool::getDocument() noexcept
{
  return drawState->getDocument();
}

const Document* DrawTool::getDocument() const noexcept
{
  return drawState->getDocument();
}

const float* DrawTool::getPrimaryColor() const noexcept
{
  return drawState->getDrawPanel()->getPrimaryColor();
}

float* DrawTool::getPrimaryColor() noexcept
{
  return drawState->getDrawPanel()->getPrimaryColor();
}

int DrawTool::getPixelSize() const noexcept
{
  return drawState->getDrawPanel()->getPixelSize();
}

BlendMode DrawTool::getBlendMode() const noexcept
{
  return drawState->getDrawPanel()->getBlendMode();
}

std::size_t DrawTool::requireCurrentLayer()
{
  return drawState->requireCurrentLayer();
}

const Image* DrawTool::getImage() const noexcept
{
  return drawState->getApp()->getImage();
}

} // namespace px
