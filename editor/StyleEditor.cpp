#include "StyleEditor.hpp"

#include <imgui.h>

namespace px {

void StyleEditor::frame(Observer* observer)
{
  Observer nullObserver;

  observer = observer ? observer : &nullObserver;

  ImGui::Begin("Custom Style Editor", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

#if 0

  if (ImGui::ColorEdit3("Background Color", backgroundColor)) {
    observer->observe(Event::ChangedBackgroundColor);
  }

  if (ImGui::ColorEdit3("Checkerboard Color", checkerboardColor)) {
    observer->observe(Event::ChangedCheckerboardColor);
  }

  if (ImGui::SliderFloat("Checkerboard Contrast", &checkerboardContrast, 0, 1)) {
    observer->observe(Event::ChangedCheckerboardContrast);
  }

  if (ImGui::SliderFloat("Checkerboard Opacity", &checkerboardColor[3], 0, 1)) {
    observer->observe(Event::ChangedCheckerboardColor);
  }

#endif

  ImGui::ShowStyleEditor();

  ImGui::End();
}

} // namespace px
