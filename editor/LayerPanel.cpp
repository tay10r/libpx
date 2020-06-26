#include "LayerPanel.hpp"

#include <libpx.hpp>

#include "App.hpp"

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
  /// Whether or not the name of the layer is being edited.
  bool editing = false;
  /// A buffer used for renaming the layer.
  std::string renameBuffer;
};

} // namespace

/// Contains the implementation data for the layer panel.
class LayerPanelImpl final
{
  friend LayerPanel;
  /// The array of layer states.
  std::vector<LayerState> states;

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
  void renderLayerList(App* app)
  {
    states.resize(getLayerCount(app->getDocument()));

    for (std::size_t i = 0; i < getLayerCount(app->getDocument()); i++) {

      ImGui::PushID(i);

      renderLayer(i, app);

      ImGui::PopID();
    }
  }
  /// Renders the properties of a specific layer.
  void renderLayerProperties(std::size_t index, App* app)
  {
    auto* layer = getLayer(app, index);
    auto opacity = getLayerOpacity(layer);
    auto visible = getLayerVisibility(layer);

    if (ImGui::SliderFloat("Opacity", &opacity, 0, 1)) {

      if (ImGui::IsItemActivated()) {
        app->snapshotDocument();
      }

      setLayerOpacity(getLayer(app, index), opacity);
    }

    if (ImGui::IsItemDeactivatedAfterEdit()) {
      app->stashDocument();
    }

    if (ImGui::Checkbox("Visible", &visible)) {

      if (ImGui::IsItemActivated()) {
        app->snapshotDocument();
      }

      setLayerVisibility(getLayer(app, index), visible);
    }

    if (ImGui::IsItemDeactivatedAfterEdit()) {
      app->stashDocument();
    }
  }
protected:
  Layer* getLayer(App* app, std::size_t index)
  {
    return px::getLayer(app->getDocument(), index);
  }
  /// Renders a single layer.
  void renderLayer(std::size_t i, App* app)
  {
    const auto& state = states[i];

    if (state.editing) {
      renderEditingLayer(i, app);
    } else {
      renderNonEditingLayer(i, app);
    }
  }
  /// Renders a layer that is being edited.
  void renderEditingLayer(std::size_t index, App* app)
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
      renameLayer(index, state.renameBuffer.c_str(), app);
      state.renameBuffer.clear();
    }

    ImGui::SameLine();

    if (ImGui::Button("Cancel")) {
      state.editing = false;
      state.renameBuffer.clear();
    }
  }
  /// Renders a layer that is not being edited.
  void renderNonEditingLayer(std::size_t index, App* app)
  {
    auto& state = states[index];

    const auto* name = getLayerName(getLayer(app, index));

    if (ImGui::Selectable(name, state.selected, ImGuiSelectableFlags_AllowDoubleClick)) {

      unselectAllLayersExcept(index);

      if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
        state.editing = true;
        state.renameBuffer.clear();
        state.renameBuffer = name;
      }
    }

#if 0
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
#endif
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
  void renameLayer(std::size_t index, const char* name, App* app)
  {
    app->snapshotDocument();

    setLayerName(getLayer(app, index), name);

    app->stashDocument();
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

bool LayerPanel::getSelectedLayer(std::size_t* index) const noexcept
{
  return self->getCurrentLayerIndex(index);
}

void LayerPanel::frame(App* app)
{
  if (ImGui::CollapsingHeader("Layers")) {
    self->renderLayerList(app);
  }

  if (ImGui::Button("Add")) {
    // TODO
  }

  ImGui::SameLine();

  if (ImGui::Button("Remove")) {
    // TODO
  }

  std::size_t selectedIndex = 0;

  if (self->getCurrentLayerIndex(&selectedIndex)) {

    ImGui::Separator();

    self->renderLayerProperties(selectedIndex, app);
  }
}

} // namespace px
