#ifndef LIBPX_EDITOR_LAYER_PANEL_HPP
#define LIBPX_EDITOR_LAYER_PANEL_HPP

#include <cstddef>

namespace px {

struct Document;

class App;
class LayerPanelImpl;

/// Used for displaying the layer stack
/// and modifying layer parameters.
class LayerPanel final
{
  /// A pointer to the implementation data.
  LayerPanelImpl* self = nullptr;
public:
  /// Constructs a new instance of the layer panel.
  LayerPanel();
  /// Releases memory allocated by the layer panel.
  ~LayerPanel();
  /// Gets the index of the currently selected layer.
  ///
  /// @param index Is assigned the current layer index.
  ///
  /// @return True on success, false if there
  /// is no currently selected layer.
  bool getSelectedLayer(std::size_t* index) const noexcept;
  /// Renders a frame on the layer panel.
  ///
  /// @param app A pointer to the application to get the document from.
  void frame(App* app);

  LayerPanel(const LayerPanel&) = delete;
};

} // namespace px

#endif // LIBPX_EDITOR_LAYER_PANEL_HPP
