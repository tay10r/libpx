#include "LayerPanel.hpp"

#include <libpx.hpp>

#include <imgui.h>
#include <imgui_stdlib.h>

#include <vector>

namespace px {

namespace {

/// Contains state information on a
/// layer in the layer panel.
struct LayerState final
{
  /// Whether or not the layer is selected.
  bool selected = false;
  /// Wether or not the layer is getting edited.
  bool editing = false;
  /// Whether or not the user is currently modifying the opacity.
  bool opacityChanging = false;
  /// The opacity of this layer.
  float opacity = 1.0;
  /// Whether or not this layer is visible.
  bool visible = true;
  /// The name of this layer.
  std::string name;
  /// A temporary buffer used for renaming the layer.
  std::string renameBuffer;
};

} // namespace

/// Contains the implementation data for the layer panel.
class LayerPanelImpl final
{
  friend LayerPanel;
  /// A type definition for a layer panel observer.
  using Observer = LayerPanel::Observer;
  /// The array of layer states.
  std::vector<LayerState> states;

  LayerPanelImpl()
  {
    // test data
    states.resize(4);
    states[0].name = "layer 1";
    states[1].name = "layer 2";
    states[2].name = "layer 3";
    states[3].name = "layer 4";
  }

  /// Gets the index of the currently selected layer.
  ///
  /// @return True if the index was found,
  /// false if there is not layer selected.
  bool getCurrentLayerIndex(std::size_t* index) const noexcept
  {
    for (std::size_t i = 0; i < states.size(); i++) {
      if (states[i].selected) {
        *index = i;
        return true;
      }
    }

    return false;
  }
  /// Renders the list of layers in the panel.
  void renderLayerList(Observer* observer)
  {
    for (std::size_t i = 0; i < states.size(); i++) {

      ImGui::PushID(i);

      renderLayer(i, observer);

      ImGui::PopID();
    }
  }
  /// Renders the properties of a specific layer.
  void renderLayerProperties(std::size_t index, Observer* observer)
  {
    auto& state = states[index];

    auto mouseButtonState = ImGui::IsMouseDown(ImGuiMouseButton_Left);

    if (ImGui::SliderFloat("Opacity", &state.opacity, 0, 1)) {
      observer->observeLayerOpacity(index, state.opacity, !state.opacityChanging);
      state.opacityChanging = true;
    }

    if (state.opacityChanging && !mouseButtonState) {
      state.opacityChanging = false;
    }

    if (ImGui::Checkbox("Visible", &state.visible)) {
      observer->observeLayerVisibility(index, state.visible);
    }
  }
protected:
  /// Renders a single layer.
  void renderLayer(std::size_t i, Observer* observer)
  {
    const auto& state = states[i];

    if (state.editing) {
      renderEditingLayer(i, observer);
    } else {
      renderNonEditingLayer(i, observer);
    }
  }
  /// Renders a layer that is being edited.
  void renderEditingLayer(std::size_t index, Observer* observer)
  {
    auto& state = states[index];

    auto flags = ImGuiInputTextFlags_EnterReturnsTrue;

    auto enterPressed = false;

    auto change = ImGui::InputText("Rename Layer", &state.renameBuffer, flags);
    if (change) {
      if (isEnterPressed()) {
        enterPressed = true;
      }
    }

    ImGui::SameLine();

    if (ImGui::Button("Commit") || enterPressed) {

      state.editing = false;
      state.name = state.renameBuffer;

      observer->observeLayerRename(index, state.name.c_str());
    }

    ImGui::SameLine();

    if (ImGui::Button("Cancel")) {
      state.editing = false;
    }
  }
  /// Renders a layer that is not being edited.
  void renderNonEditingLayer(std::size_t index, Observer* observer)
  {
    auto& state = states[index];

    const auto* name = state.name.c_str();

    if (ImGui::Selectable(name, state.selected, ImGuiSelectableFlags_AllowDoubleClick)) {

      unselectAllLayersExcept(index);

      if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
        state.editing = true;
        state.renameBuffer.clear();
        state.renameBuffer = state.name;
      }
    }

    ImGuiDragDropFlags sourceFlags = 0;
    sourceFlags |= ImGuiDragDropFlags_SourceNoDisableHover;
    sourceFlags |= ImGuiDragDropFlags_SourceNoHoldToOpenOthers;

    if (ImGui::BeginDragDropSource(sourceFlags)) {
      ImGui::SetDragDropPayload("pxLayerMove", &index, sizeof(index));
      ImGui::EndDragDropSource();
    }

    ImGuiDragDropFlags targetFlags = 0;
    targetFlags |= ImGuiDragDropFlags_AcceptBeforeDelivery;
    targetFlags |= ImGuiDragDropFlags_AcceptNoDrawDefaultRect;

    if (ImGui::BeginDragDropTarget()) {

      const auto* payload = ImGui::AcceptDragDropPayload("pxLayerMove", targetFlags);
      if (payload) {
        std::size_t dstIndex = *((const std::size_t*) payload->Data);
        moveLayer(index, dstIndex);
        observer->observeLayerMove(index, dstIndex);
      }

      ImGui::EndDragDropTarget();
    }
  }
  /// Unselects all but one layer.
  /// This is required since when the artist
  /// draws, they are drawing to just one layer.
  void unselectAllLayersExcept(std::size_t index)
  {
    for (std::size_t i = 0; i < states.size(); i++) {
      states[i].selected = (i == index);
    }
  }
  /// Moves a layer.
  void moveLayer(std::size_t src, std::size_t dst)
  {
    auto tmp = std::move(states.at(src));

    for (std::size_t i = src; i < dst; i++) {
      states.at(i) = std::move(states.at(i + 1));
    }

    for (std::size_t i = src; i > dst; i--) {
      states.at(i) = std::move(states.at(i - 1));
    }

    states.at(dst) = std::move(tmp);
  }
  // Detects if the 'enter' button is pressed.
  static bool isEnterPressed()
  {
    return ImGui::IsKeyDown(ImGui::GetKeyIndex(ImGuiKey_Enter));
  }
};

LayerPanel::LayerPanel() : self(new LayerPanelImpl()) {}

LayerPanel::~LayerPanel()
{
  delete self;
}

void LayerPanel::addLayer(const char* name)
{
  LayerState state;

  state.name = name;

  self->states.emplace_back(std::move(state));
}

void LayerPanel::removeLayer(std::size_t index)
{
  if (index >= self->states.size()) {
    return;
  }

  self->states.erase(self->states.begin() + index);
}

bool LayerPanel::getSelectedLayer(std::size_t* index) const noexcept
{
  return self->getCurrentLayerIndex(index);
}

void LayerPanel::frame(Observer* observer)
{
  ImGui::Begin("Layer Panel", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

  if (ImGui::CollapsingHeader("Layers")) {
    self->renderLayerList(observer);
  }

  if (ImGui::Button("Add")) {
    observer->observe(Event::ClickedAdd);
  }

  ImGui::SameLine();

  if (ImGui::Button("Remove")) {
    observer->observe(Event::ClickedRemove);
  }

  std::size_t selectedIndex = 0;

  if (self->getCurrentLayerIndex(&selectedIndex)) {

    ImGui::Separator();

    self->renderLayerProperties(selectedIndex, observer);
  }

  ImGui::End();
}

void LayerPanel::selectLayer(std::size_t index) noexcept
{
  for (std::size_t i = 0; i < self->states.size(); i++) {
    self->states[i].selected = (i == index);
  }
}

void LayerPanel::sync(const Document* doc)
{
  std::size_t layerCount = getLayerCount(doc);

  self->states.clear();

  self->states.resize(layerCount);

  for (std::size_t i = 0; i < self->states.size(); i++) {
    const auto* layer = getLayer(doc, i);
    self->states[i].name = getLayerName(layer);
    self->states[i].opacity = getLayerOpacity(layer);
    self->states[i].visible = getLayerVisibility(layer);
  }
}

} // namespace px
