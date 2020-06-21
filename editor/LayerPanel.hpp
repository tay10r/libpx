#ifndef LIBPX_EDITOR_LAYER_PANEL_HPP
#define LIBPX_EDITOR_LAYER_PANEL_HPP

#include <cstddef>

namespace px {

struct Document;

class LayerPanelImpl;

/// Used for displaying the layer stack
/// and modifying layer parameters.
class LayerPanel final
{
  /// A pointer to the implementation data.
  LayerPanelImpl* self = nullptr;
public:
  /// Enumerates the events (simple ones)
  /// that can occur on the layer panel.
  enum class Event
  {
    /// The 'Add' button was clicked.
    ClickedAdd,
    /// The 'Remove' button was clicked.
    ClickedRemove
  };
  /// Used for observing various events from
  /// the layer panel.
  class Observer
  {
  public:
    /// Just a stub.
    virtual ~Observer() {}
    /// Observes a button event from the layer panel.
    virtual void observe(Event) {}
    /// Observes a move operation on the layer panel.
    virtual void observeLayerMove(std::size_t, std::size_t) {}
    /// Observes a layer getting renamed.
    virtual void observeLayerRename(std::size_t, const char*) {}
    /// Observes a change in layer opacity.
    virtual void observeLayerOpacity(std::size_t, float, bool) {}
    /// Observes a change in layer visibility.
    virtual void observeLayerVisibility(std::size_t, bool) {}
  };
  /// Constructs a new instance of the layer panel.
  LayerPanel();
  /// Releases memory allocated by the layer panel.
  ~LayerPanel();
  /// Adds a layer to the layer panel.
  ///
  /// @param name The name of the layer to add.
  void addLayer(const char* name);
  /// Removes a layer from the layer panel.
  ///
  /// @param index The index of the layer to remove.
  void removeLayer(std::size_t index);
  /// Gets the index of the currently selected layer.
  ///
  /// @param index Is assigned the current layer index.
  ///
  /// @return True on success, false if there
  /// is no currently selected layer.
  bool getSelectedLayer(std::size_t* index) const noexcept;
  /// Renders a frame on the layer panel.
  ///
  /// @param observer A pointer to the observer of layer panel events.
  void frame(Observer* observer);
  /// Selects a layer by index.
  ///
  /// @param index The index of the layer to select.
  void selectLayer(std::size_t index) noexcept;
  /// Synchronizes the layer panel with
  /// a given document.
  ///
  /// @param doc The document to synchronize the layer with.
  void sync(const Document* doc);

  LayerPanel(const LayerPanel&) = delete;
};

} // namespace px

#endif // LIBPX_EDITOR_LAYER_PANEL_HPP
