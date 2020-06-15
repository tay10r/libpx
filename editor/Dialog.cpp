#include "Dialog.hpp"

#include <imgui.h>

namespace px {

bool Dialog::render(Editor* editor)
{
  if (!started) {
    ImGui::OpenPopup(name);
    started = true;
  }

  if (ImGui::BeginPopupModal(name, nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
    renderInner(editor);
    ImGui::EndPopup();
    return true;
  } else {
    return false;
  }
}

void Dialog::close()
{
  ImGui::CloseCurrentPopup();
}

} // namespace px
