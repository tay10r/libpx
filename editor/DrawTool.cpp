#include "DrawTool.hpp"

#include "Editor.hpp"

namespace px {

void DrawTool::snapshotDoc()
{
  editor->snapshotDoc();
}

Document* DrawTool::getDocument() noexcept
{
  return editor->getDocument();
}

const Document* DrawTool::getDocument() const noexcept
{
  return editor->getDocument();
}

const Image* DrawTool::getImage() const noexcept
{
  return editor->getImage();
}

} // namespace px