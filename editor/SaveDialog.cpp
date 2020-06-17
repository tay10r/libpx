#include "SaveDialog.hpp"

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

class SaveDialog final : public Dialog
{
  char directory[4096];
  char filename[4096];
  std::unique_ptr<pfd::select_folder> dialog;
public:
  SaveDialog() : Dialog("Save Document")
  {
    std::memset(directory, 0, sizeof(directory));
    std::memset(filename, 0, sizeof(filename));
    std::strcat(filename, "untitled.px");
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

    if (ImGui::Button("Save") && !dialog) {
      saveDoc(editor);
      close();
    }

    ImGui::SameLine();

    if (ImGui::Button("Cancel") && !dialog) {
      editor->setDiscardChanges(true);
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
  void saveDoc(Editor* editor)
  {
    std::string fullPath(directory);

    if (!fullPath.empty()) {
      fullPath += dirSeparator();
    }

    fullPath += filename;

    if (!fullPath.empty()) {
      editor->saveDoc(fullPath.c_str());
    }
  }
};

} // namespace

Dialog* createSaveDialog()
{
  return new SaveDialog();
}

} // namespace px
