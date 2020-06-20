#include "App.hpp"

#include "AppState.hpp"
#include "DocumentProperties.hpp"
#include "DrawPanel.hpp"
#include "History.hpp"
#include "Input.hpp"
#include "Log.hpp"
#include "MenuBar.hpp"
#include "Platform.hpp"
#include "Renderer.hpp"

#include <libpx.hpp>

#include <imgui.h>

#include <glm/glm.hpp>

#include <memory>
#include <vector>

#include <cstdio>

namespace px {

namespace {

/// Represents the application when it is being used
/// for drawing the artwork.
class DrawState final : public AppState,
                        public DrawPanel::Observer
{
  DrawPanel drawPanel;
public:
  DrawState(App* app) : AppState(app) {}
  /// Renders the draw state windows.
  void frame() override
  {
    renderDocument();

    const auto* menuBar = getMenuBar();

    if (menuBar->drawPanelVisible()) {
      drawPanel.frame(this);
    }
  }
protected:
  /// Renders the document onto the window.
  void renderDocument()
  {
    auto* image = getApp()->getImage();

    auto* color = getColorBuffer(image);
    auto w = getImageWidth(image);
    auto h = getImageHeight(image);

    getPlatform()->getRenderer()->blit(color, w, h);
  }
  /// Observes an event from the draw panel.
  void observe(DrawPanel::Event event) override
  {
    switch (event) {
      case DrawPanel::Event::ChangedBlendMode:
        break;
      case DrawPanel::Event::ChangedPixelSize:
        break;
      case DrawPanel::Event::ChangedPrimaryColor:
        break;
      case DrawPanel::Event::ChangedTool:
        break;
    }
  }
};

/// Implements the interface to the application.
class AppImpl final : public App,
                      public MenuBar::Observer,
                      public DocumentProperties::Observer
{
  /// The document history stack.
  History history;
  /// A pointer to the image that the
  /// document is rendered to.
  Image* image = nullptr;
  /// The stack of app states.
  /// The last element is the top of the stack.
  std::vector<std::unique_ptr<AppState>> stateStack;
  /// The menu bar attached to the window.
  MenuBar menuBar;
  /// The properties of the currently opened document.
  DocumentProperties docProperties;
  /// The log for events and errors.
  Log log;
public:
  /// Constructs a new app instance.
  AppImpl(Platform* p) : App(p), image(createImage(64, 64))
  {
    pushAppState(new DrawState(this));
  }
  /// Releases memory allocated by the app.
  ~AppImpl()
  {
    closeImage(image);
  }
  /// Gets a pointer the log.
  Log* getLog() noexcept override
  {
    return &log;
  }
  /// Gets a pointer to the current document snapshot.
  Document* getDocument() noexcept override
  {
    return history.getDocument();
  }
  /// Gets a pointer to the current document snapshot.
  const Document* getDocument() const noexcept override
  {
    return history.getDocument();
  }
  /// Takes a snapshot of the current document.
  void snapshotDocument() override
  {
    history.snapshot();
  }
  /// Gets a pointer to the menu bar.
  Image* getImage() noexcept override
  {
    return image;
  }
  /// Gets a pointer to the menu bar.
  const Image* getImage() const noexcept override
  {
    return image;
  }
  /// Gets a pointer to the menu bar.
  const MenuBar* getMenuBar() const noexcept override
  {
    return &menuBar;
  }
  /// Gets a pointer to the menu bar.
  MenuBar* getMenuBar() noexcept override
  {
    return &menuBar;
  }
  /// Checks for any non-options that may be interpreted
  /// as a document to be opened.
  bool parseArgs(int, char**) override
  {
    return true;
  }
  /// Pushes a state to the stack.
  void pushAppState(AppState* state) override
  {
    stateStack.emplace_back(state);
  }
  /// Renders a frame of the application.
  bool frame() override
  {
    try {
      uncheckedFrame();
    } catch (...) {
      return false;
    }

    return true;
  }
  /// Handles mouse motion events.
  void mouseMotion(const MouseMotion&) override
  {
    //std::printf("mouse: %d %d\n", x, y);
  }
  /// Handles mouse button state changes.
  void mouseButton(const MouseButton& button) override
  {
    (void)button;
    std::printf("here\n");
  }
protected:
  /// This function renders a frame without checking for
  /// exceptions.
  ///
  /// Exceptions are checked by the calling function.
  void uncheckedFrame()
  {
    getPlatform()->getRenderer()->clear(1, 1, 1, 1);

    menuBar.frame(this);

    if (menuBar.documentPropertiesVisible()) {
      docProperties.frame();
    }

    if (menuBar.logVisible()) {
      log.frame();
    }

    for (auto& state : stateStack) {
      state->frame();
    }
  }
  /// Observes a menu bar event.
  void observe(MenuBar::Event event) override
  {
    switch (event) {
      case MenuBar::Event::ClickedClose:
        break;
      case MenuBar::Event::ClickedOpen:
        break;
      case MenuBar::Event::ClickedSave:
        break;
      case MenuBar::Event::ClickedSaveAs:
        break;
      case MenuBar::Event::ClickedExportSpriteSheet:
        break;
      case MenuBar::Event::ClickedExportZip:
        break;
      case MenuBar::Event::ClickedExportCurrentFrame:
        break;
      case MenuBar::Event::ClickedRedo:
        break;
      case MenuBar::Event::ClickedUndo:
        break;
      case MenuBar::Event::ClickedQuit:
        break;
      case MenuBar::Event::ClickedTheme:
        break;
      case MenuBar::Event::ClickedCustomTheme:
        break;
    }
  }
  /// Observers a document properties event.
  void observe(DocumentProperties::Event event) override
  {
    switch (event) {
      case DocumentProperties::Event::ChangeBackgroundColor:
        break;
      case DocumentProperties::Event::ChangeDirectory:
        break;
      case DocumentProperties::Event::ChangeSize:
        break;
      case DocumentProperties::Event::ChangeName:
        break;
      case DocumentProperties::Event::ClickedDirectoryBrowse:
        break;
    }
  }
};

} // namesapce

App* App::init(Platform* platform) { return new AppImpl(platform); }

} // namespace px
