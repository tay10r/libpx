#ifndef LIBPX_EDITOR_DRAW_STATE_HPP
#define LIBPX_EDITOR_DRAW_STATE_HPP

#include "AppState.hpp"

#include <libpx.hpp>

namespace px {

class DrawPanel;
class LayerPanel;

/// Represents a state in the application
/// for drawing onto the document.
class DrawState : public AppState
{
public:
  /// Initializes the draw state.
  ///
  /// @param app A pointer to the application
  /// instance that the draw state is for.
  static DrawState* init(App* app);
  /// Initializes the base draw state.
  DrawState(App* app) : AppState(app) {}
  /// Just a stub.
  virtual ~DrawState() {}
  /// Gets a pointer to the draw panel.
  /// This is used by the drawing tools to
  /// get common tool information, such as
  /// pixel size or color.
  ///
  /// @return A pointer to the draw panel.
  virtual const DrawPanel* getDrawPanel() const noexcept = 0;
  /// Gets a non-const pointer to the draw panel.
  /// This is used particularly by the color picker tool
  /// since the tool is used to change the primary color
  /// in the draw panel.
  ///
  /// @return A non-const pointer to the draw panel.
  virtual DrawPanel* getDrawPanel() noexcept = 0;
  /// Gets a pointer to the layer panel.
  /// This is used by most tools to figure out
  /// which layer is currently selected.
  ///
  /// @return A pointer to the layer panel.
  virtual const LayerPanel* getLayerPanel() const noexcept = 0;
  /// This function is called when a layer index
  /// is required for an operation to continue.
  /// The original justification for this function
  /// is for draw tools that create primitives, which
  /// need a layer index to add the primitives to.
  ///
  /// @return An index for the current layer.
  virtual std::size_t requireCurrentLayer() = 0;
};

} // namespace px

#endif // LIBPX_EDITOR_DRAW_STATE_HPP
