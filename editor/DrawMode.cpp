#include "DrawMode.hpp"

#include "DrawTool.hpp"
#include "Editor.hpp"

#include "BucketTool.hpp"
#include "ColorPickerTool.hpp"
#include "EllipseTool.hpp"
#include "PenTool.hpp"
#include "RectTool.hpp"
#include "StrokeTool.hpp"

#include <libpx.hpp>

#include <imgui.h>

#include <memory>
#include <sstream>
#include <vector>

#include <cstring>

namespace px {

namespace {

/// Contains state information on a layer.
struct LayerState final
{
  /// Whether or not the layer is selected.
  bool selected = false;
  /// Wether or not the layer is getting edited.
  bool editing = false;
  /// The input buffer for renaming layers (TODO : std::string)
  char renameBuffer[4096] {};
};

/// This mode is made for drawing operations.
class DrawModeImpl final : public DrawMode
{
  /// A pointer to the editor hosting the mode.
  Editor* editor = nullptr;
  /// The currently selected tool.
  std::unique_ptr<DrawTool> currentTool;
  /// The index of the currently selected tool.
  int toolIndex = 0;
  /// Whether or not the document size is locked.
  bool sizeLock = true;
  /// The current primary color.
  float primaryColor[3] { 0, 0, 0 };
  /// The current pixel size.
  int pixelSize = 1;
  /// The last known cursor position.
  int cursor[2] { 0, 0 };
  /// The user interface states of each layer.
  std::vector<LayerState> layerStates;
public:
  /// Constructs a new instance of the draw mode.
  ///
  /// @param e A pointer to the editor that the draw mode is for.
  DrawModeImpl(Editor* e) : editor(e)
  {
    currentTool = std::unique_ptr<DrawTool>(createPenTool(this));
  }
  /// Passes the left click state change
  /// to the currently active tool.
  void leftClick(bool state) override
  {
    if (currentTool) {
      currentTool->leftClick(state);
    }
  }
  /// Passes the mouse motion event
  /// to the currently selected tool.
  void mouseMotion(unsigned x, unsigned y) override
  {
    cursor[0] = x;
    cursor[1] = y;

    if (currentTool) {
      currentTool->mouseMotion(x, y);
    }
  }
  /// Renders the mode widgets.
  void render() override
  {
    renderDrawPanel();
    renderLayers();
  }
  /// Passes the right click state change
  /// to the currently active tool.
  void rightClick(bool state) override
  {
    if (currentTool) {
      currentTool->rightClick(state);
    }
  }

  Editor* getEditor() noexcept { return editor; }
  const Editor* getEditor() const noexcept { return editor; }

  /// Gets the current pixel size.
  int getPixelSize() const noexcept override { return pixelSize; }
  /// Gets the current cursor position.
  const int* getCursor() const noexcept override { return cursor; }
  /// Gets a pointer to the document.
  Document* getDocument() noexcept override
  {
    return editor->getDocument();
  }
  /// Gets a const-pointer to the document.
  const Document* getDocument() const noexcept override
  {
    return editor->getDocument();
  }
  /// Gets the current primary color.
  float* getPrimaryColor() noexcept override { return primaryColor; }
  /// Gets the current primary color.
  const float* getPrimaryColor() const noexcept override { return primaryColor; }
  /// Gets the index to the current layer.
  std::size_t requireCurrentLayer() override
  {
    for (std::size_t i = 0; i < layerStates.size(); i++) {
      if (layerStates[i].selected) {
        return i;
      }
    }

    if (px::getLayerCount(getDocument()) == 0) {
      addLayer();
    }

    return 0;
  }
protected:
  /// Renders the drawing panel controls.
  void renderDrawPanel()
  {
    ImGui::Begin("Draw Panel", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

    if (ImGui::CollapsingHeader("Available Tools")) {

      auto hit = false;

      hit |= ImGui::RadioButton(         "Pen", &toolIndex, 0);
      hit |= ImGui::RadioButton(      "Stroke", &toolIndex, 1);
      hit |= ImGui::RadioButton(      "Bucket", &toolIndex, 2);
      hit |= ImGui::RadioButton(   "Rectangle", &toolIndex, 3);
      hit |= ImGui::RadioButton(     "Ellipse", &toolIndex, 4);
      hit |= ImGui::RadioButton("Color Picker", &toolIndex, 5);

      if (hit) {
        switch (toolIndex) {
          case 0:
            currentTool.reset(createPenTool(this));
            break;
          case 1:
            currentTool.reset(createStrokeTool(this));
            break;
          case 2:
            currentTool.reset(createBucketTool(this));
            break;
          case 3:
            currentTool.reset(createRectTool(this));
            break;
          case 4:
            currentTool.reset(createEllipseTool(this));
            break;
          case 5:
            currentTool.reset(createColorPickerTool(this));
            break;
        }
      }
    }

    if (ImGui::CollapsingHeader("Tool Properties", false)) {
      if (currentTool) {
        currentTool->renderProperties();
      }

      ImGui::InputInt("Pixel Size", &pixelSize);

      ImGui::ColorEdit3("Primary Color", primaryColor);
    }

    if (ImGui::CollapsingHeader("Document Properties", false)) {

      auto* doc = editor->getDocument();
      int w = getDocWidth(doc);
      int h = getDocHeight(doc);

      ImGui::InputInt( "Width", &w, 2, 32);
      ImGui::InputInt("Height", &h, 2, 32);
      ImGui::Checkbox("Size Lock", &sizeLock);

      if (!sizeLock) {
        resizeDoc(doc, w, h);
      }

      float bg[4];

      getBackground(doc, bg);

      if (ImGui::ColorEdit4("Background Color", bg)) {
        setBackground(doc, bg[0], bg[1], bg[2], bg[3]);
      }
    }

    ImGui::Text("Cursor: (%u, %u)", cursor[0], cursor[1]);

    ImGui::End();
  }
  /// Render the layers window.
  void renderLayers()
  {
    ImGui::Begin("Layer Panel", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

    renderLayerList();

    if (ImGui::Button("Add")) {
      addLayer();
    }

    ImGui::SameLine();

    if (ImGui::Button("Remove")) {
      removeSelectedLayers();
    }

    ImGui::End();
  }
  /// Renders the list of layers.
  void renderLayerList()
  {
    auto layerCount = getLayerCount(getDocument());

    if (layerCount != layerStates.size()) {
      updateLayerStates();
    }

    for (std::size_t i = 0; i < layerCount; i++) {

      ImGui::PushID(i);

      renderLayer(i);

      ImGui::PopID();
    }
  }
  /// Resizes the layer state table.
  void updateLayerStates()
  {
    layerStates.resize(getLayerCount(getDocument()));
  }
  /// Renders a single layer button.
  ///
  /// @param index The index of the layer.
  void renderLayer(std::size_t index)
  {
    auto& state = layerStates.at(index);

    if (state.editing) {

      auto flags = ImGuiInputTextFlags_EnterReturnsTrue;

      auto enterPressed = false;

      auto change = ImGui::InputText("Rename Layer", state.renameBuffer, sizeof(state.renameBuffer), flags);
      if (change) {
        if (isEnterPressed()) {
          enterPressed = true;
        }
      }

      ImGui::SameLine();

      if (ImGui::Button("Commit") || enterPressed) {

        editor->snapshotDoc();

        setLayerName(getLayer(index), state.renameBuffer);

        state.editing = false;

        updateLayerStates();
      }

    } else {

      const auto* name = getLayerName(getLayer(index));

      if (ImGui::Selectable(name, &state.selected, ImGuiSelectableFlags_AllowDoubleClick)) {

        unselectAllLayersExcept(index);

        if (ImGui::IsMouseDoubleClicked(0)) {
          state.editing = true;
          std::memset(state.renameBuffer, 0, sizeof(state.renameBuffer));
          std::memcpy(state.renameBuffer, name, std::min(std::strlen(name), sizeof(state.renameBuffer) - 1));
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

          px::moveLayer(getDocument(), index, dstIndex);
        }

        ImGui::EndDragDropTarget();
      }
    }
  }
  /// Unselects all but one layer.
  /// This is required since when the artist
  /// draws, they are drawing to just one layer.
  void unselectAllLayersExcept(std::size_t index)
  {
    for (std::size_t i = 0; i < layerStates.size(); i++) {
      if (i == index) {
        continue;
      } else {
        layerStates[i].selected = false;
      }
    }
  }
  // Detects if the 'enter' button is pressed.
  static bool isEnterPressed()
  {
    return ImGui::IsKeyDown(ImGui::GetKeyIndex(ImGuiKey_Enter));
  }
  /// Adds a new layer.
  /// This will also snapshot the document
  /// to track the modification.
  void addLayer()
  {
    editor->snapshotDoc();

    auto* doc = editor->getDocument();

    px::addLayer(doc);

    layerStates.emplace_back();
  }
  /// Removes all selected layers.
  /// This will also snapshot the document
  /// to track the modification.
  void removeSelectedLayers()
  {
    editor->snapshotDoc();

    // Just as a safe check, we'll make sure
    // that we have as many layer states as we
    // have layers.

    auto* doc = editor->getDocument();

    layerStates.resize(getLayerCount(doc));

    for (std::size_t i = 0; i < layerStates.size(); i++) {

      if (!layerStates[i].selected || layerStates[i].editing) {
        continue;
      }

      removeLayer(doc, i);

      layerStates.erase(layerStates.begin() + i);
    }

    updateLayerStates();
  }
  /// Gets a pointer to a layer.
  inline Layer* getLayer(std::size_t index)
  {
    return px::getLayer(getDocument(), index);
  }
  /// Gets a const-pointer to a layer.
  inline const Layer* getLayer(std::size_t index) const
  {
    return px::getLayer(getDocument(), index);
  }
};

} // namespace

DrawMode* createDrawMode(Editor* editor)
{
  return new DrawModeImpl(editor);
}

} // namespace px
