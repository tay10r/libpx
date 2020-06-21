#include "App.hpp"

#include "AppState.hpp"
#include "DocumentProperties.hpp"
#include "DrawState.hpp"
#include "History.hpp"
#include "Input.hpp"
#include "Log.hpp"
#include "MenuBar.hpp"
#include "Platform.hpp"
#include "Renderer.hpp"
#include "StyleEditor.hpp"

#include <libpx.hpp>

#include <imgui.h>

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include <memory>
#include <vector>

#include <cstdio>

namespace px {

namespace {

/// Implements the interface to the application.
class AppImpl final : public App,
                      public MenuBar::Observer,
                      public DocumentProperties::Observer,
                      public StyleEditor::Observer
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
  /// Used for editing the application style.
  StyleEditor styleEditor;
  /// The translation of the document on the edit area.
  //float translation[2] { 0, 0 };
  /// The current zoom factor.
  float zoom = 1;
public:
  /// Constructs a new app instance.
  AppImpl(Platform* p) : App(p), image(createImage(64, 64))
  {
    pushAppState(DrawState::init(this));
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
  /// Gets the current zoom factor.
  float getZoom() const noexcept override
  {
    return zoom;
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
  /// Handles a keyboard event.
  void key(const KeyEvent& keyEvent) override
  {
    if (keyEvent.isCtrlKey('z') && keyEvent.state) {
      undo();
      return;
    } else if ((keyEvent.isCtrlKey('y') || keyEvent.isCtrlShiftKey('z')) && keyEvent.state) {
      redo();
      return;
    } else if (keyEvent.isKey('+') && keyEvent.state) {
      zoomIn();
    } else if (keyEvent.isKey('-') && keyEvent.state) {
      zoomOut();
    }

    if (stateStack.empty()) {
      return;
    }

    stateStack[stateStack.size() - 1]->key(keyEvent);
  }
  /// Handles mouse motion events.
  void mouseMotion(const MouseMotionEvent& motion) override
  {
    if (stateStack.empty()) {
      return;
    }

    stateStack[stateStack.size() - 1]->mouseMotion(motion);
  }
  /// Handles mouse button state changes.
  void mouseButton(const MouseButtonEvent& button) override
  {
    if (stateStack.empty()) {
      return;
    }

    stateStack[stateStack.size() - 1]->mouseButton(button);
  }
protected:
  /// This function renders a frame without checking for
  /// exceptions.
  ///
  /// Exceptions are checked by the calling function.
  void uncheckedFrame()
  {
    const auto& bg = ImGui::GetStyleColorVec4(ImGuiCol_WindowBg);

    auto* renderer = getPlatform()->getRenderer();

    renderer->clear(bg.x, bg.y, bg.z, bg.w);

    renderer->setCheckerboardColor(1, 1, 1, 0.8);

    renderer->setCheckerboardContrast(0.8);

    menuBar.frame(this);

    if (menuBar.documentPropertiesVisible()) {
      docProperties.frame(this);
    }

    if (menuBar.styleEditorVisible()) {
      styleEditor.frame(this);
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
        redo();
        break;
      case MenuBar::Event::ClickedUndo:
        undo();
        break;
      case MenuBar::Event::ClickedQuit:
        break;
      case MenuBar::Event::ClickedTheme:
        updateTheme();
        break;
      case MenuBar::Event::ClickedCustomTheme:
        break;
      case MenuBar::Event::ClickedZoomIn:
        zoomIn();
        break;
      case MenuBar::Event::ClickedZoomOut:
        zoomOut();
        break;
    }
  }
  /// Observers a document properties event.
  void observe(DocumentProperties::Event event) override
  {
    switch (event) {
      case DocumentProperties::Event::ChangeBackgroundColor:
        updateDocumentBackgroundColor();
        break;
      case DocumentProperties::Event::ChangeDirectory:
        break;
      case DocumentProperties::Event::ChangeSize:
        updateDocumentSize();
        break;
      case DocumentProperties::Event::ChangeName:
        break;
      case DocumentProperties::Event::ClickedDirectoryBrowse:
        break;
    }
  }
  /// Observers an event from the style editor.
  void observe(StyleEditor::Event event) override
  {
    auto* renderer = getPlatform()->getRenderer();

    switch (event) {
      case StyleEditor::Event::ChangedBackgroundColor:
        break;
      case StyleEditor::Event::ChangedCheckerboardColor:
        renderer->setCheckerboardColor(styleEditor.getCheckerboardColor());
        break;
      case StyleEditor::Event::ChangedCheckerboardContrast:
        renderer->setCheckerboardContrast(styleEditor.getCheckerboardContrast());
        break;
    }
  }
  /// Undoes a document change.
  void undo()
  {
    history.undo();
  }
  /// Redoes a document change.
  void redo()
  {
    history.redo();
  }
  /// Zooms in by the default factor.
  void zoomIn(float factor = 2)
  {
    zoom *= factor;
  }
  /// Zooms out by the default factor.
  void zoomOut(float factor = 2)
  {
    zoom /= factor;
  }
  /// Updates the document size based on
  /// what is in the document properties panel.
  void updateDocumentSize()
  {
    snapshotDocument();

    auto w = docProperties.getWidth();
    auto h = docProperties.getHeight();

    resizeDoc(getDocument(), w, h);

    resizeImage(image, w, h);
  }
  /// Updates the background color of the document.
  void updateDocumentBackgroundColor()
  {
    snapshotDocument();

    const auto* bg = docProperties.getBackgroundColor();

    setBackground(getDocument(), bg);
  }
  /// Updates the theme based on the user selection.
  void updateTheme()
  {
    const char* style = menuBar.getSelectedTheme();

    auto isStyle = [style](const char* other) {
      return !!(std::strcmp(style, other) == 0);
    };

    if (isStyle("Dark")) {
      ImGui::StyleColorsDark();
    } else if (isStyle("Light")) {
      ImGui::StyleColorsLight();
    }
  }
};

} // namesapce

App* App::init(Platform* platform)
{
  return new AppImpl(platform);
}

} // namespace px
