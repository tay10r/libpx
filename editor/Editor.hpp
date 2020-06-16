#ifndef LIBPX_EDITOR_EDITOR_HPP
#define LIBPX_EDITOR_EDITOR_HPP

namespace px {

class Document;
class EditorImpl;
class Image;

/// Represents the application as a whole.
/// Contains the main window used to interface
/// with the user.
class Editor final {
  /// Contains the implementation data of the editor.
  EditorImpl* impl = nullptr;
public:
  /// Partially initializes the editor.
  /// The functions @ref Editor::createWindow and
  /// @ref Editor::initGl must be called afterwards.
  Editor();
  ~Editor();
  /// Initializes the GL data.
  ///
  /// @return True on success, false on failure.
  bool initGlData();
  /// Iterates the event loop.
  void iterate();
  /// Saves the current frame as a PNG file.
  ///
  /// @param filename The path to save the file at.
  ///
  /// @return True on success, false on failure.
  bool savePNG(const char* filename) const noexcept;
  /// Indicates whether or not the editor
  /// should exit.
  ///
  /// @return True if the editor should exit,
  /// false if it should not.
  bool shouldExit();
  /// Creates the main window.
  ///
  /// @return True on success, false on failure.
  bool createWindow();
  /// Accesses a pointer to the document.
  Document* getDocument() noexcept;
  /// Accesses a pointer to the document.
  const Document* getDocument() const noexcept;
  /// Creates a "snapshot" of the document in the edit
  /// history. This is done when a modification
  /// is about to be made.
  void snapshotDoc();
  /// Gets a pointer to the last rendered image.
  ///
  /// @return A pointer to the last rendered image.
  const Image* getImage() const noexcept;
  /// Handles mouse motion.
  ///
  /// @param x The X coordinate of the new mouse position.
  /// @param y The Y coordinate of the new mouse position.
  void mouseMotion(double x, double y);
  /// Handles a left click state change.
  ///
  /// @param state The state of the left click button.
  void leftClick(bool state);
  /// Handles a right click state change.
  ///
  /// @param state The state of the right click button.
  void rightClick(bool state);
protected:
  /// Renders a new frame.
  void renderFrame();

  /// Renders the user interface.
  void renderUi();
  /// Renders the menu bar at the top of the window.
  void renderMenuBar();
  /// Renders the file menu.
  void renderFileMenu();
  /// Renders the edit menu.
  void renderEditMenu();
  /// Renders the view menu.
  void renderViewMenu();

  /// Renders the document being edited.
  void renderDocument();

  /// Switches to the light color theme.
  void lightMode();
  /// Switches to the dark color theme.
  void darkMode();
};

} // namespace px

#endif // LIBPX_EDITOR_EDITOR_HPP
