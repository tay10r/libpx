#ifndef LIBPX_EDITOR_TOOL_HPP
#define LIBPX_EDITOR_TOOL_HPP

namespace px {

class Document;
class Editor;

/// The base of any tool that can be used
/// in the editor.
class Tool
{
  /// A pointer to the editor using the tool.
  Editor* editor = nullptr;
public:
  /// Constructs a new tool instance.
  ///
  /// @param e A pointer to the editor instance.
  Tool(Editor* e) : editor(e) {}
  /// Just a stub.
  virtual ~Tool() {}
  /// Handles mouse motion.
  ///
  /// @param x The X coordinate within the window.
  /// @param y The Y coordinate within the window.
  virtual void mouseMotion(unsigned x, unsigned y) = 0;
  /// Handles a change in state of the left mouse button.
  ///
  /// @param state The new state of the left click button.
  virtual void leftClick(bool state) = 0;
  /// Handles a change in state of the right mouse button.
  ///
  /// @param state The new state of the right click button.
  virtual void rightClick(bool state) = 0;
  /// Renders the tool properties.
  virtual void renderProperties() = 0;
  /// Accesses a reference to the document.
  inline Document* getDocument() { return editor->getDocument(); }
  /// Accesses a reference to the document.
  inline const Document* getDocument() const { return editor->getDocument(); }
};

} // namespace px

#endif /* LIBPX_EDITOR_TOOL_HPP */
