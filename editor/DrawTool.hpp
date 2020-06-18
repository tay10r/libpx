#ifndef LIBPX_EDITOR_DRAW_TOOL_HPP
#define LIBPX_EDITOR_DRAW_TOOL_HPP

#include <cstddef>

namespace px {

class Document;
class DrawMode;
class Editor;
class Image;

/// The base of any tool that can be used
/// in the editor in draw mode.
class DrawTool
{
  /// A pointer to the draw mode.
  DrawMode* drawMode = nullptr;
public:
  /// Constructs a new tool instance.
  ///
  /// @param d A pointer to the draw mode
  /// instance that's hosting the tool.
  DrawTool(DrawMode* d) : drawMode(d) {}
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
protected:
  /// Creates a snapshot of the document
  /// for editing purposes.
  void snapshotDoc();
  /// Gets a pointer to the document.
  Document* getDocument() noexcept;
  /// Gets a pointer to the document.
  const Document* getDocument() const noexcept;
  /// Gets a pointer to the last rendered image.
  const Image* getImage() const noexcept;
  /// Gets a pointer to the primary color.
  float* getPrimaryColor() noexcept;
  /// Gets a pointer to the primary color.
  const float* getPrimaryColor() const noexcept;
  /// Gets a pointer to the cursor position.
  const int* getCursor() const noexcept;
  /// Gets the current pixel size.
  int getPixelSize() const noexcept;
  /// Gets the index of the current layer.
  std::size_t requireCurrentLayer();
};

} // namespace px

#endif /* LIBPX_EDITOR_DRAW_TOOL_HPP */
