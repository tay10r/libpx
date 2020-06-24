#include "InternalErrorState.hpp"

#include "App.hpp"
#include "Log.hpp"
#include "Platform.hpp"

#include <imgui.h>

namespace px {

namespace {

/// The URL used to report issues with.
const char* reportBugURL = "https://github.com/tay10r/libpx/issues/new?assignees=&labels=bug&template=internal-error.md&title=";

} // namespace

void InternalErrorState::frame()
{
  ImGui::OpenPopup("Internal Error");

  if (ImGui::BeginPopupModal("Internal Error", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {

    ImGui::Text("An error has occurred that has caused the editor stop working.");

    ImGui::Text("");

    ImGui::Text("Please report this issue to https://github.com/tay10r/libpx");

    ImGui::Text("");

    if (ImGui::Button("Copy URL to Clipboard")) {
      ImGui::SetClipboardText(reportBugURL);
    }

    ImGui::SameLine();

    if (ImGui::Button("Copy Log to Clipboard")) {
      getApp()->getLog()->copyToClipboard();
    }

    ImGui::SameLine();

    if (ImGui::Button("Quit Editor")) {
      getPlatform()->quit();
    }

    ImGui::EndPopup();
  }
}

} // namespace px
