#ifndef LIBPX_EDITOR_DRAW_TOOL_HPP
#define LIBPX_EDITOR_DRAW_TOOL_HPP

#include <cstddef>

#include <libpx.hpp>

namespace px {

struct Document;
struct Image;
struct MouseButtonEvent;
struct MouseMotionEvent;

class DrawState;

/// The base of any tool that can be used
/// in the editor in draw mode.
class DrawTool
{
  /// A pointer to the draw state.
  DrawState* drawState = nullptr;
  /// Whether or not the draw tool
  /// is actively editing the document.
  /// This happens while the user is
  /// holding down the mouse button
  /// on the document.
  bool active = false;
public:
  /// Constructs a new tool instance.
  ///
  /// @param d A pointer to the draw state
  /// instance that's hosting the tool.
  DrawTool(DrawState* d) : drawState(d) {}
  /// Just a stub.
  virtual ~DrawTool() {}
  /// Indicates whether the draw tool is active.
  /// This is used to determine whether to
  /// call @ref DrawTool::begin or @ref DrawTool::end.
  inline bool isActive() const noexcept
  {
    return active;
  }
  /// Whether or not this tool should be
  /// called for left click events. This
  /// should probably always be true.
  virtual bool isLeftClickTool() const noexcept
  {
    return true;
  }
  /// Whether or not this tool should be
  /// called for right click events.
  virtual bool isRightClickTool() const noexcept
  {
    return false;
  }
  /// Called when the user left clicks a location in document space.
  ///
  /// @param mouseButton A reference to the original mouse button event.
  /// @param docX The X coordinate of the click position, in document space.
  /// @param docY The Y coordinate of the click position, in document space.
  inline void begin(const MouseButtonEvent& mouseButton, int docX, int docY)
  {
    active = true;
    onBegin(mouseButton, docX, docY);
  }
  /// Called when, after clicking in document space, the user drags the cursor
  /// to a new area. This function is only called if the user continues holding
  /// down the mouse button.
  inline void drag(const MouseMotionEvent& mouseMotion, int docX, int docY)
  {
    onDrag(mouseMotion, docX, docY);
  }
  /// Called when user lets go of the mouse button.
  inline void end(int docX, int docY)
  {
    active = false;
    onEnd(docX, docY);
  }
  /// Renders the tool properties.
  virtual void renderProperties() {}
protected:
  /// Derived to implement the tool behavior.
  virtual void onBegin(const MouseButtonEvent&, int, int) = 0;
  /// Derived to implement the tool behavior.
  virtual void onDrag(const MouseMotionEvent&, int, int) {}
  /// Derived to implement the tool behavior.
  virtual void onEnd(int, int) {}
  /// Creates a snapshot of the document.
  void snapshotDocument();
  /// Stashes changes to a document.
  void stashDocument();
  /// Gets a non-const pointer to the document.
  Document* getDocument() noexcept;
  /// Gets a pointer to the document.
  const Document* getDocument() const noexcept;
  /// Accesses the primary color.
  float* getPrimaryColor() noexcept;
  /// Accesses the primary color.
  const float* getPrimaryColor() const noexcept;
  /// Gets the current pixel size.
  int getPixelSize() const noexcept;
  /// Gets the current blend mode.
  BlendMode getBlendMode() const noexcept;
  /// Gets the index of the current layer.
  std::size_t requireCurrentLayer();
  /// Gets a pointer to the last rendered image.
  const Image* getImage() const noexcept;
};

} // namespace px

#endif /* LIBPX_EDITOR_DRAW_TOOL_HPP */
