#include "DrawPanel.hpp"

#include <imgui.h>

namespace px {

namespace {

/// Used as a stub to prevent
/// null-checks at every event.
class NullObserver final : public DrawPanel::Observer
{
public:
  void observe(DrawPanel::Event) override {}
};

/// Used for mapping tools to their names.
/// Additionally used for enumerating tools
/// as integer indices.
struct ToolTableEntry final
{
  /// The tool enumeration value.
  DrawPanel::Tool tool;
  /// The name of the tool.
  const char* name = "";
};

const ToolTableEntry toolTable[] {
  { DrawPanel::Tool::Pen,         "Pen" },
  { DrawPanel::Tool::Eraser,      "Eraser" },
  { DrawPanel::Tool::Stroke,      "Stroke" },
  { DrawPanel::Tool::Bucket,      "Bucket" },
  { DrawPanel::Tool::Rectangle,   "Rectangle" },
  { DrawPanel::Tool::Ellipse,     "Ellipse" },
  { DrawPanel::Tool::ColorPicker, "Color Picker" }
};

constexpr std::size_t toolCount() noexcept
{
  return sizeof(toolTable) / sizeof(toolTable[0]);
}

int getToolIndex(DrawPanel::Tool tool) noexcept
{
  for (std::size_t i = 0; i < toolCount(); i++) {
    if (tool == toolTable[i].tool) {
      return int(i);
    }
  }
  return 0;
}

struct BlendModeTableEntry final
{
  BlendMode blendMode;
  const char* name = "";
};

const BlendModeTableEntry blendModeTable[] {
  { BlendMode::Normal,   "Normal" },
  { BlendMode::Subtract, "Subtract" }
};

constexpr std::size_t blendModeCount() noexcept
{
  return sizeof(blendModeTable) / sizeof(blendModeTable[0]);
}

const char* getBlendModeName(BlendMode blendMode) noexcept
{
  for (std::size_t i = 0; i < blendModeCount(); i++) {
    if (blendModeTable[i].blendMode == blendMode) {
      return blendModeTable[i].name;
    }
  }

  return "";
}

} // namespace

void DrawPanel::frame(Observer* observer)
{
  NullObserver nullObserver;

  observer = observer ? observer : &nullObserver;

  ImGui::Begin("Draw Panel", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

  availableTools(observer);
  toolProperties(observer);

  ImGui::End();
}

void DrawPanel::availableTools(Observer* observer)
{
  if (ImGui::CollapsingHeader("Available Tools")) {

    int index = getToolIndex(currentTool);

    for (std::size_t i = 0; i < toolCount(); i++) {

      if (ImGui::RadioButton(toolTable[i].name, &index, int(i))) {

        currentTool = toolTable[i].tool;

        observer->observe(Event::ChangedTool);
      }
    }
  }
}

void DrawPanel::toolProperties(Observer* observer)
{
  if (ImGui::CollapsingHeader("Tool Properties")) {

    if (ImGui::SliderInt("Pixel Size", &pixelSize, 0, 8)) {
      observer->observe(Event::ChangedPixelSize);
    }

    if (ImGui::ColorEdit4("Primary Color", primaryColor)) {
      observer->observe(Event::ChangedPrimaryColor);
    }

    if (ImGui::BeginCombo("Blend Mode", getBlendModeName(blendMode))) {

      for (std::size_t i = 0; i < blendModeCount(); i++) {

        if (ImGui::Selectable(blendModeTable[i].name)) {
          blendMode = blendModeTable[i].blendMode;
          observer->observe(Event::ChangedBlendMode);
        }
      }

      ImGui::EndCombo();
    }
  }
}

} // namespace px
