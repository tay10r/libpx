#ifndef LIBPX_EDITOR_MENU_BAR_HPP
#define LIBPX_EDITOR_MENU_BAR_HPP

namespace px {

/// Represents the menu bar at the top of the window.
class MenuBar final
{
  /// Contains the state information
  /// on what is visible and what is not.
  struct VisibilityState final
  {
    /// Whether or not the draw panel is visible.
    bool drawPanel = true;
    /// Whether or not the document properties are visible.
    bool docProperties = true;
    /// Whether or not the log is visible.
    bool log = false;
    /// The style editor widget.
    /// There's not a checkbox for this,
    /// since it's not useful for drawing.
    bool styleEditor = false;
  };
  /// The one and only instance of @ref VisibilityState.
  VisibilityState visibility;
  /// Indicates the current theme being used.
  const char* currentTheme = "Dark";
public:
  /// Enumerates the observable events in the menu bar.
  enum class Event
  {
    ClickedClose,
    ClickedExportCurrentFrame,
    ClickedExportSpriteSheet,
    ClickedExportZip,
    ClickedOpen,
    ClickedRedo,
    ClickedSave,
    ClickedSaveAs,
    ClickedUndo,
    ClickedQuit,
    ClickedCustomTheme,
    ClickedTheme
  };
  /// Used for observing events from the menu bar.
  class Observer
  {
  public:
    /// Just a stub.
    virtual ~Observer() {}
    /// Observes an event from the menu bar.
    virtual void observe(Event) = 0;
  };
  /// Renders a frame of the menu bar.
  void frame(Observer* observer = nullptr);
  /// Indicates whether or not the draw panel should be visible.
  inline bool drawPanelVisible() const noexcept
  {
    return visibility.drawPanel;
  }
  /// Indicates whether or not the document properties should be visible.
  inline bool documentPropertiesVisible() const noexcept
  {
    return visibility.docProperties;
  }
  /// Indicates whether or not the log is visible.
  inline bool logVisible() const noexcept
  {
    return visibility.log;
  }
  /// Gets the name of the currently selected theme.
  inline const char* getSelectedTheme() const noexcept
  {
    return currentTheme;
  }
protected:
  /// Renders the file menu.
  void fileMenu(Observer*);
  /// Renders the edit menu.
  void editMenu(Observer*);
  /// Renders the view menu.
  void viewMenu(Observer*);
};

} // namespace px

#endif // LIBPX_EDITOR_MENU_BAR_HPP
