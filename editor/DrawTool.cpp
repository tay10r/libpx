#include "DrawTool.hpp"

#include "DrawMode.hpp"
#include "Editor.hpp"

namespace px {

void DrawTool::snapshotDoc()
{
  drawMode->getEditor()->snapshotDoc();
}

Document* DrawTool::getDocument() noexcept
{
  return drawMode->getEditor()->getDocument();
}

const Document* DrawTool::getDocument() const noexcept
{
  return drawMode->getEditor()->getDocument();
}

const Image* DrawTool::getImage() const noexcept
{
  return drawMode->getEditor()->getImage();
}

float* DrawTool::getPrimaryColor() noexcept
{
  return drawMode->getPrimaryColor();
}

BlendMode DrawTool::getBlendMode() const noexcept
{
  return drawMode->getBlendMode();
}

const float* DrawTool::getPrimaryColor() const noexcept
{
  return drawMode->getPrimaryColor();
}

const int* DrawTool::getCursor() const noexcept
{
  return drawMode->getCursor();
}

int DrawTool::getPixelSize() const noexcept
{
  return drawMode->getPixelSize();
}

std::size_t DrawTool::requireCurrentLayer()
{
  return drawMode->requireCurrentLayer();
}

} // namespace px
