#include "MenuBar.hpp"

#include <imgui.h>

namespace px {

namespace {

class NullObserver final : public MenuBar::Observer
{
public:
  void observe(MenuBar::Event) override {}
};

} // namespace

void MenuBar::frame(Observer* observer)
{
  NullObserver nullObserver;

  observer = observer ? observer : &nullObserver;

  if (ImGui::BeginMainMenuBar()) {

    fileMenu(observer);
    editMenu(observer);
    viewMenu(observer);

    ImGui::EndMainMenuBar();
  }
}

void MenuBar::fileMenu(Observer* observer)
{
  if (!ImGui::BeginMenu("File")) {
    return;
  }

  if (ImGui::MenuItem("Open", "Ctrl+O")) {
    observer->observe(Event::ClickedOpen);
  }

  if (ImGui::MenuItem("Save", "Ctrl+S")) {
    observer->observe(Event::ClickedSave);
  }

  if (ImGui::MenuItem("Save as PX File")) {
    observer->observe(Event::ClickedSaveAsPx);
  }

  if (ImGui::BeginMenu("Export")) {

    if (ImGui::MenuItem("As Sprite Sheet", nullptr, false, false)) {
      observer->observe(Event::ClickedExportSpriteSheet);
    }

    if (ImGui::MenuItem("As Zip", nullptr, false, false), false) {
      observer->observe(Event::ClickedExportZip);
    }

    if (ImGui::MenuItem("Current Frame")) {
      observer->observe(Event::ClickedExportCurrentFrame);
    }

    ImGui::EndMenu();
  }

  if (ImGui::MenuItem("Quit")) {
    observer->observe(Event::ClickedQuit);
  }

  ImGui::EndMenu();
}

void MenuBar::editMenu(Observer* observer)
{
  if (!ImGui::BeginMenu("Edit")) {
    return;
  }

  if (ImGui::MenuItem("Undo", "Ctrl+Z")) {
    observer->observe(Event::ClickedUndo);
  }

  if (ImGui::MenuItem("Redo", "Ctrl+Shift+Z")) {
    observer->observe(Event::ClickedRedo);
  }

  ImGui::EndMenu();
}

void MenuBar::viewMenu(Observer* observer)
{
  if (!ImGui::BeginMenu("View")) {
    return;
  }

  if (ImGui::MenuItem("Zoom In", "+")) {
    observer->observe(Event::ClickedZoomIn);
  }

  if (ImGui::MenuItem("Zoom Out", "-")) {
    observer->observe(Event::ClickedZoomOut);
  }

  ImGui::Separator();

  if (ImGui::BeginMenu("Theme")) {

    if (ImGui::MenuItem("Light")) {
      currentTheme = "Light";
      observer->observe(Event::ClickedTheme);
    }

    if (ImGui::MenuItem("Dark")) {
      currentTheme = "Dark";
      observer->observe(Event::ClickedTheme);
    }

#if 0 // TODO : for later release
    if (ImGui::MenuItem("Custom...")) {
      observer->observe(Event::ClickedCustomStyle);
    }
#endif

    ImGui::EndMenu();
  }

  ImGui::Separator();

  ImGui::Checkbox("Draw Panel", &visibility.drawPanel);

  ImGui::Checkbox("Layer Panel", &visibility.layerPanel);

  ImGui::Checkbox("Document Properties", &visibility.docProperties);

  ImGui::Checkbox("Log", &visibility.log);

  ImGui::Checkbox("Style Editor", &visibility.styleEditor);

  ImGui::EndMenu();
}

} // namespace px
