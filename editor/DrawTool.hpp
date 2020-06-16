#ifndef LIBPX_EDITOR_DRAW_TOOL_HPP
#define LIBPX_EDITOR_DRAW_TOOL_HPP

#include "DrawState.hpp"

namespace px {

struct DrawState;

class Document;
class Editor;

/// The base of any tool that can be used
/// in the editor in draw mode.
class DrawTool
{
  /// A pointer to the editor using the tool.
  Editor* editor = nullptr;
  /// The draw state passed to the constructor.
  DrawState drawState;
public:
  /// Constructs a new tool instance.
  ///
  /// @param e A pointer to the editor instance.
  DrawTool(Editor* e, const DrawState& ds) : editor(e), drawState(ds) {}
  /// Just a stub.
  virtual ~DrawTool() {}
  /// Handles mouse motion.
  ///
  /// @param x The X coordinate within the window.
  /// @param y The Y coordinate within the window.
  virtual void mouseMotion(unsigned x, unsigned y) { (void)x; (void)y; }
  /// Handles a change in state of the left mouse button.
  ///
  /// @param state The new state of the left click button.
  virtual void leftClick(bool state) = 0;
  /// Handles a change in state of the right mouse button.
  ///
  /// @param state The new state of the right click button.
  virtual void rightClick(bool state) { (void) state; }
  /// Renders the tool properties.
  virtual void renderProperties() {}
  /// Accesses a reference to the document.
  Document* getDocument() noexcept;
  /// Accesses a reference to the document.
  const Document* getDocument() const noexcept;
protected:
  /// Gets a reference to the draw state.
  inline const DrawState& getDrawState() const noexcept { return drawState; }
  /// Gets a reference to the draw state.
  inline DrawState& getDrawState() noexcept { return drawState; }
  /// Gets a pointer to the primary color.
  inline float* getPrimaryColor() const noexcept {
    return drawState.primaryColor;
  }
  /// Gets a pointer to the cursor position.
  inline int* getCursor() const noexcept {
    return drawState.cursor;
  }
  /// Gets the current pixel size.
  inline int getPixelSize() const noexcept {
    return *drawState.pixelSize;
  }
};

} // namespace px

#endif /* LIBPX_EDITOR_DRAW_TOOL_HPP */
