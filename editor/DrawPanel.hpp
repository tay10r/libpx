#ifndef LIBPX_EDITOR_DRAW_PANEL_HPP
#define LIBPX_EDITOR_DRAW_PANEL_HPP

#include <libpx.hpp>

namespace px {

/// Contains state information on the
/// draw operations as well as document properties.
class DrawPanel final
{
public:
  /// Enumerates the tools shown in
  /// the draw panel.
  enum class Tool
  {
    Pen,
    Eraser,
    Stroke,
    Bucket,
    Rectangle,
    Ellipse,
    ColorPicker
  };
  /// Enumerates events that can occur when the
  /// user interacts with the draw panel.
  enum class Event
  {
    ChangedBlendMode,
    ChangedPrimaryColor,
    ChangedPixelSize,
    ChangedTool
  };
  /// An interface to observe events from the draw panel.
  class Observer
  {
  public:
    /// Just a stub.
    virtual ~Observer() {}
    /// Called when an event occurs in the draw panel.
    virtual void observe(Event) {}
  };
private:
  /// The current primary color.
  float primaryColor[4] { 0, 0, 0, 1 };
  /// The current tool being used.
  Tool currentTool = Tool::Pen;
  /// How thick strokes should be.
  int pixelSize = 1;
  /// How strokes should be blended.
  BlendMode blendMode = BlendMode::Normal;
public:
  /// Renders the draw panel.
  ///
  /// @param observer An optional pointer to observe
  /// changes that occur.
  void frame(Observer* observer = nullptr);
protected:
  /// Renders the available tools.
  void availableTools(Observer*);
  /// Renders the tool properties.
  void toolProperties(Observer*);
};

} // namespace px

#endif // LIBPX_EDITOR_DRAW_PANEL_HPP
