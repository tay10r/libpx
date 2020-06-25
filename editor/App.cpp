#include "App.hpp"

#include "AppState.hpp"
#include "AppStorage.hpp"
#include "Blob.hpp"
#include "BrowseDocumentsState.hpp"
#include "DocumentProperties.hpp"
#include "DrawState.hpp"
#include "History.hpp"
#include "ImageIO.hpp"
#include "Input.hpp"
#include "InternalErrorState.hpp"
#include "LocalStorage.hpp"
#include "Log.hpp"
#include "MenuBar.hpp"
#include "OpenErrorState.hpp"
#include "Platform.hpp"
#include "Renderer.hpp"
#include "StyleEditor.hpp"

#include <libpx.hpp>

#include <imgui.h>

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include <filesystem>
#include <memory>
#include <vector>

#include <cstdio>

namespace px {

namespace {

/// Implements the interface to the application.
class AppImpl final : public App,
                      public AppStorage::Observer,
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
  /// The ID of the currently edited document.
  int documentID = -1;
public:
  /// Constructs a new app instance.
  AppImpl(Platform* p) : App(p), image(createImage(64, 64))
  {
    AppStorage::init(this);
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
  /// Causes the application to fail due to internal reasons.
  void internallyFail() override
  {
    stateStack.emplace_back(new InternalErrorState(this));
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
    } else if (keyEvent.isCtrlKey('s') && keyEvent.state) {
      saveDocumentToAppStorage();
    } else if (keyEvent.isCtrlShiftKey('s') && keyEvent.state) {
      saveDocumentToLocalStorage();
    } else if (keyEvent.isCtrlKey('w') && keyEvent.state) {
      // close
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
  /// Creates a new document.
  void createDocument() override
  {
    history = History();

    syncDocument();

    documentID = AppStorage::createDocument();
  }
  /// Opens a document by a given path.
  ///
  /// @param id The ID of the document to open.
  ///
  /// @return True on success, false on failure.
  bool openDocument(int id) override
  {
    documentID = id;

    Document* doc = createDoc();

    history = History(doc);

    ErrorList* errList = nullptr;

    int err = AppStorage::openDocument(id, doc, &errList);

    syncDocument();

    docProperties.setDocumentName(AppStorage::getDocumentName(id).c_str());

    if (err != 0) {
      pushAppState(new OpenErrorState(this, err, errList));
      return false;
    }

    return true;
  }
  /// Stashes any unsaved changes to the document.
  void stashDocument() override
  {
    AppStorage::stashDocument(documentID, getDocument());
  }
  /// Removes a document from application storage.
  ///
  /// @param id The ID of the document to open.
  void removeDocument(int id) override
  {
    AppStorage::removeDocument(id);
    AppStorage::syncToDevice(this);
  }
  /// Synchronizes all editor data with new document data.
  void syncDocument()
  {
    const auto* doc = history.getDocument();

    resizeImage(image, getDocWidth(doc), getDocHeight(doc));

    docProperties.sync(doc);
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

    renderer->setCheckerboardColor(1, 1, 1, 1);

    renderer->setCheckerboardContrast(0.2);

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

    if (!stateStack.empty()) {

      auto* currentState = stateStack[stateStack.size() - 1].get();

      currentState->frame();

      if (currentState->shouldClose()) {
        stateStack.pop_back();
      }
    }
  }
  /// Observes a menu bar event.
  void observe(MenuBar::Event event) override
  {
    switch (event) {
      case MenuBar::Event::ClickedClose:
        break;
      case MenuBar::Event::ClickedSave:
        saveDocumentToAppStorage();
        break;
      case MenuBar::Event::ClickedExportPx:
        saveDocumentToLocalStorage();
        break;
      case MenuBar::Event::ClickedExportSpriteSheet:
        break;
      case MenuBar::Event::ClickedExportZip:
        break;
      case MenuBar::Event::ClickedExportCurrentFrame:
        exportCurrentFrame();
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
      case DocumentProperties::Event::ChangeSize:
        updateDocumentSize();
        break;
    }
  }
  /// Observes the document getting renamed.
  ///
  /// @param name The name that the document was given.
  void observeDocumentRename(const char* name) override
  {
    AppStorage::renameDocument(documentID, name);
    AppStorage::syncToDevice(this);
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
  /// Observers a synchronization result from app storage.
  ///
  /// @param msg A pointer to the error message, if an error ocurred.
  /// If the operation was successfull, this is a null pointer.
  void observeSyncResult(const char* msg) override
  {
    if (msg != nullptr) {
      log.logError("Failed to synchronize app storage: ", msg);
      internallyFail();
    } else if (stateStack.empty()) {
      // First sync call means we're initializing the application.
      // In the future, there should probably be different derived
      // classes for each sync operation.
      pushAppState(BrowseDocumentsState::init(this));
    }
  }
  /// Saves the document to the application storage.
  void saveDocumentToAppStorage()
  {
    AppStorage::saveDocument(documentID, getDocument());
    AppStorage::syncToDevice(this);
  }
  /// Saves the document to local storage.
  void saveDocumentToLocalStorage()
  {
    const char* docName = docProperties.getDocumentName();

    void* data = nullptr;

    size_t size = 0;

    saveDoc(getDocument(), &data, &size);

    std::string filename = std::string(docName) + ".px";

    LocalStorage::save(filename.c_str(), data, size);

    std::free(data);
  }
  /// Writes a PNG file containing the current frame.
  void exportCurrentFrame()
  {
    Blob blob = formatPNG(image);

    LocalStorage::save("Untitled.png", blob.data(), blob.size());
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
