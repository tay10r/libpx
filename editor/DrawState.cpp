#include "DrawState.hpp"

#include "App.hpp"
#include "DrawPanel.hpp"
#include "Input.hpp"
#include "LayerPanel.hpp"
#include "MenuBar.hpp"
#include "Platform.hpp"
#include "Renderer.hpp"

#include "BucketTool.hpp"
#include "ColorPickerTool.hpp"
#include "EllipseTool.hpp"
#include "EraserTool.hpp"
#include "PenTool.hpp"
#include "RectTool.hpp"
#include "StrokeTool.hpp"

#include <libpx.hpp>

#include <imgui.h>

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include <memory>

#include <cstdio>

namespace px {

namespace {

/// Represents the application when it is being used
/// for drawing the artwork.
class DrawStateImpl final : public DrawState,
                            public DrawPanel::Observer,
                            public LayerPanel::Observer
{
  /// Contains state information on
  /// the drawing tools.
  DrawPanel drawPanel;
  /// Shows all the layers from the document.
  LayerPanel layerPanel;
  /// The current draw tool.
  std::unique_ptr<DrawTool> currentTool;
public:
  DrawStateImpl(App* app) : DrawState(app)
  {
    currentTool.reset(new PenTool(this));
    layerPanel.sync(getDocument());
  }
  /// Renders the draw state windows.
  void frame() override
  {
    renderDocument();

    const auto* menuBar = getMenuBar();

    if (menuBar->drawPanelVisible()) {
      drawPanel.frame(this);
    }

    if (menuBar->layerPanelVisible()) {
      layerPanel.frame(this);
    }
  }
  /// Handles a mouse button event.
  void mouseButton(const MouseButtonEvent& mouseButton) override
  {
    if (!currentTool) {
      return;
    }

    auto usable = (currentTool->isLeftClickTool() && mouseButton.isLeft())
               || (currentTool->isRightClickTool() && mouseButton.isRight());

    if (!usable) {
      return;
    }

    auto cursor = ImGui::GetIO().MousePos;

    auto docPos = windowToDoc(glm::vec2(cursor.x, cursor.y));

    if (!currentTool->isActive() && mouseButton.isPressed()) {
      currentTool->begin(mouseButton, docPos.x, docPos.y);
    } else if (currentTool->isActive() && mouseButton.isReleased()) {
      currentTool->end(docPos.x, docPos.y);
    }
  }
  /// Handles mouse motion event.
  void mouseMotion(const MouseMotionEvent& motion) override
  {
    auto pos = windowToDoc(glm::vec2(motion.x, motion.y));

    if (currentTool && currentTool->isActive()) {
      currentTool->drag(motion, pos.x, pos.y);
    }

    getPlatform()->getRenderer()->setCursor(pos.x, pos.y);
  }
  /// Gets a pointer to the draw panel.
  const DrawPanel* getDrawPanel() const noexcept override
  {
    return &drawPanel;
  }
  /// Gets a non-const pointer to the draw panel.
  DrawPanel* getDrawPanel() noexcept override
  {
    return &drawPanel;
  }
  /// Gets a pointer to the layer panel.
  ///
  /// @return A pointer to the layer panel.
  const LayerPanel* getLayerPanel() const noexcept override
  {
    return &layerPanel;
  }
  /// Requires that a layer be available for editing.
  /// This function will create a layer if it doesn't
  /// already exist.
  std::size_t requireCurrentLayer() override
  {
    std::size_t layerIndex = 0;

    if (layerPanel.getSelectedLayer(&layerIndex)) {
      return layerIndex;
    }

    auto* doc = getDocument();
    if (!getLayerCount(doc)) {
      px::addLayer(doc);
    }

    layerPanel.selectLayer(0);

    return layerIndex;
  }
protected:
  /// Gets the current window size.
  glm::vec2 getWinSize() noexcept
  {
    std::size_t w = 0;
    std::size_t h = 0;
    getPlatform()->getWindowSize(&w, &h);
    return glm::vec2(float(w), float(h));
  }
  /// Gets the size of the current document snapshot.
  glm::vec2 getDocSize() const noexcept
  {
    const auto* doc = getDocument();
    return glm::vec2(float(getDocWidth(doc)),
                     float(getDocHeight(doc)));
  }
  /// Calculates the transformation to be applied
  /// based on the editing position, zoom factor,
  /// and aspect ratios.
  glm::mat4 calculateTransform() noexcept
  {
    float zoom = getApp()->getZoom();

    std::size_t fbW = 0;
    std::size_t fbH = 0;
    getPlatform()->getWindowSize(&fbW, &fbH);

    const auto* doc = getDocument();
    std::size_t docW  = getDocWidth(doc);
    std::size_t docH = getDocHeight(doc);

    float aspectA = float(fbW) / fbH;
    float aspectB = float(docW) / docH;

    float scaleX = zoom * (aspectB / aspectA);
    float scaleY = zoom;

    return glm::scale(glm::vec3(scaleX, scaleY, 1.0f));
  }
  /// Gets a pointer to the platform interface.
  Platform* getPlatform() noexcept
  {
    return getApp()->getPlatform();
  }
  /// Gets a pointer to the current document snapshot.
  Document* getDocument() noexcept
  {
    return getApp()->getDocument();
  }
  /// Gets a pointer to the current document snapshot.
  const Document* getDocument() const noexcept
  {
    return getApp()->getDocument();
  }
  /// Renders the document onto the window.
  void renderDocument()
  {
    auto* renderer = getPlatform()->getRenderer();

    auto transform = calculateTransform();

    renderer->setTransform(glm::value_ptr(transform));

    auto* doc = getDocument();

    auto* image = getApp()->getImage();

    render(doc, image);

    auto* color = getColorBuffer(image);
    auto w = getImageWidth(image);
    auto h = getImageHeight(image);

    renderer->blit(color, w, h);
  }
  /// Observes an event from the draw panel.
  void observe(DrawPanel::Event event) override
  {
    switch (event) {
      case DrawPanel::Event::ChangedBlendMode:
        break;
      case DrawPanel::Event::ChangedPixelSize:
        break;
      case DrawPanel::Event::ChangedPrimaryColor:
        break;
      case DrawPanel::Event::ChangedTool:
        updateTool();
        break;
    }
  }
  /// Observes an event from the layer panel.
  void observe(LayerPanel::Event event) override
  {
    switch (event) {
      case LayerPanel::Event::ClickedAdd:
        addLayer();
        break;
      case LayerPanel::Event::ClickedRemove:
        removeLayer();
        break;
    }
  }
  /// Observes the moving of a layer.
  ///
  /// @param src The source index.
  /// @param dst The destination index.
  void observeLayerMove(std::size_t src, std::size_t dst) override
  {
    snapshotDocument();

    moveLayer(getDocument(), src, dst);
  }
  /// Observes a layer getting renamed.
  ///
  /// @param index The index of the renamed layer.
  /// @param name The name given to the layer.
  void observeLayerRename(std::size_t index, const char* name) override
  {
    snapshotDocument();

    auto* layer = getLayer(getDocument(), index);

    setLayerName(layer, name);
  }
  /// Observes a change in the layer opacity.
  ///
  /// @param index The index of the layer whose opacity is being changed.
  /// @param opacity The new opacity value.
  /// @param firstChange Whether or not this is the first change
  /// while the user is holding down the button.
  void observeLayerOpacity(std::size_t index, float opacity, bool firstChange) override
  {
    if (firstChange) {
      snapshotDocument();
    }

    auto* layer = getLayer(getDocument(), index);

    setLayerOpacity(layer, opacity);
  }
  /// Observes a change in the layer's visibility.
  ///
  /// @param index The index of the layer.
  /// @param visible The new visibility state.
  void observeLayerVisibility(std::size_t index, bool visible) override
  {
    snapshotDocument();

    auto* layer = getLayer(getDocument(), index);

    setLayerVisibility(layer, visible);
  }
  /// Changes the currently selected tool,
  /// based on what's indicating in the draw panel.
  void updateTool()
  {
    currentTool.reset();

    switch (drawPanel.getCurrentTool()) {
      case DrawPanel::Tool::Bucket:
        currentTool.reset(new BucketTool(this));
        break;
      case DrawPanel::Tool::ColorPicker:
        currentTool.reset(new ColorPickerTool(this));
        break;
      case DrawPanel::Tool::Ellipse:
        currentTool.reset(new EllipseTool(this));
        break;
      case DrawPanel::Tool::Eraser:
        currentTool.reset(new EraserTool(this));
        break;
      case DrawPanel::Tool::Pen:
        currentTool.reset(new PenTool(this));
        break;
      case DrawPanel::Tool::Rectangle:
        currentTool.reset(new RectTool(this));
        break;
      case DrawPanel::Tool::Stroke:
        currentTool.reset(new StrokeTool(this));
        break;
    }
  }
  /// Converts a position in window space to a position
  /// in document space.
  ///
  /// @param in The position in window space to convert.
  ///
  /// @return The resultant position in document space.
  glm::vec2 windowToDoc(const glm::vec2& in) noexcept
  {
    auto winSize = getWinSize();

    auto transform = calculateTransform();

    glm::vec4 scaledSize = transform * glm::vec4(winSize.x, winSize.y, 0, 1);

    auto offset = (winSize - glm::vec2(scaledSize.x, scaledSize.y)) * 0.5f;

    auto docSize = getDocSize();

    auto pos = in - offset;

    pos = glm::vec2(pos.x * docSize.x / scaledSize.x,
                    pos.y * docSize.y / scaledSize.y);

    return pos;
  }
  /// Converts NDC coordinates to document coordinates.
  ///
  /// @param in The NDC coordinates to convert.
  ///
  /// @return The NDC coordinates converted to document coordinates.
  glm::vec2 ndcToDocument(const glm::vec2& in) noexcept
  {
    auto w = getDocWidth(getDocument());
    auto h = getDocHeight(getDocument());

    return glm::vec2 {
      ((in[0] + 1) * 0.5) * w,
      ((1 - in[1]) * 0.5) * h
    };
  }
#if 0
  glm::vec2 docToNDC(const glm::vec2& in) noexcept
  {
  }
#endif
  /// Converts window coordinates to NDC coordinates.
  ///
  /// @param in The window coordinates to convert.
  ///
  /// @return The window coordinates as normalized coordinates.
  glm::vec2 windowToNDC(const glm::vec2& in) noexcept
  {
    std::size_t w = 0;
    std::size_t h = 0;

    getPlatform()->getWindowSize(&w, &h);

    glm::vec2 uv { in[0] / float(w),
                   in[1] / float(h) };

    return glm::vec2 {
      (2 * uv[0]) - 1,
      1 - (2 * uv[1])
    };
  }
  /// Adds a layer to the document.
  void addLayer()
  {
    auto* layer = px::addLayer(getDocument());

    layerPanel.addLayer(getLayerName(layer));
  }
  /// Removes the currently selected layer from the document.
  void removeLayer()
  {
    std::size_t currentLayer = 0;

    if (!layerPanel.getSelectedLayer(&currentLayer)) {
      return;
    }

    layerPanel.removeLayer(currentLayer);

    px::removeLayer(getDocument(), currentLayer);
  }
};

} // namesapce

DrawState* DrawState::init(App* app)
{
  return new DrawStateImpl(app);
}

} // namespace px
