#include "ExportDialog.hpp"

#include "Dialog.hpp"
#include "Editor.hpp"

#include <imgui.h>

#include <portable-file-dialogs.h>

#include <cstring>

namespace px {

namespace {

constexpr char dirSeparator() noexcept
{
#ifdef _WIN32
  return '\\';
#else
  return '/';
#endif
}

class ExportDialog final : public Dialog
{
  char directory[4096];
  char filename[4096];
  std::unique_ptr<pfd::select_folder> dialog;
public:
  ExportDialog() : Dialog("Export Dialog")
  {
    std::memset(filename, 0, sizeof(filename));
    std::strcat(filename, "untitled.png");
  }
protected:
  void renderInner(Editor* editor) override
  {
    if (dialog) {
      checkBrowseResult();
    }

    ImGui::InputText("Directory", directory, sizeof(directory), ImGuiInputTextFlags_ReadOnly);

    ImGui::SameLine();

    if (ImGui::Button("Browse") && !dialog) {
      browse();
    }

    ImGui::InputText("Filename", filename, sizeof(filename));

    if (ImGui::Button("Export") && !dialog) {
      savePNG(editor);
      close();
    }

    ImGui::SameLine();

    if (ImGui::Button("Cancel") && !dialog) {
      close();
    }
  }
  void browse()
  {
    if (dialog) {
      return;
    }

    dialog = std::unique_ptr<pfd::select_folder>(new pfd::select_folder("Select Directory"));
  }
  void checkBrowseResult()
  {
    if (!dialog) {
      return;
    }

    if (dialog->ready()) {

      auto tmp = dialog->result();
      if (tmp.size() >= (sizeof(directory) - 1)) {
        return;
      }

      std::memcpy(directory, tmp.data(), tmp.size());

      dialog.reset();
    }
  }
  void savePNG(Editor* editor)
  {
    std::string fullPath(directory);

    if (!fullPath.empty()) {
      fullPath += dirSeparator();
    }

    fullPath += filename;

    if (!fullPath.empty()) {
      editor->savePNG(fullPath.c_str());
    }
  }
};

} // namespace

Dialog* createExportDialog()
{
  return new ExportDialog();
}

} // namespace px
