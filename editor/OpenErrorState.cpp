#include "OpenErrorState.hpp"

#include <libpx.hpp>

#include <imgui.h>

#include <cstring>

namespace px {

OpenErrorState::~OpenErrorState()
{
  if (errList) {
    closeErrorList(errList);
    errList = nullptr;
  }
}

void OpenErrorState::frame()
{
  ImGui::Begin("Open Failure", &openFlag, ImGuiWindowFlags_AlwaysAutoResize);

  if (errList) {
    ImGui::Text("Failed to open document (Syntax Errors)");
  } else {
    ImGui::Text("Failed to open document (%s)", std::strerror(errnoCopy));
  }

  ImGui::Text("");

  std::size_t errCount = errList ? getErrorCount(errList) : 0;

  for (std::size_t i = 0; i < errCount; i++) {

    auto line = getErrorLine(errList, i);
    auto column = getErrorColumn(errList, i);
    const auto* desc = getErrorDescription(errList, i);

    ImGui::Text("(line:%u column:%u): %s",
                unsigned(line),
                unsigned(column),
                desc);
  }

  if (!openFlag) {
    queueClose();
  }

  ImGui::End();
}

} // namespace px
