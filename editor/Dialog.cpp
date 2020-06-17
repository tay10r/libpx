#include "Dialog.hpp"

#include <imgui.h>

#include <portable-file-dialogs.h>

namespace px {

class DialogImpl final
{
  friend Dialog;
  /// The name of the dialog window.
  const char* name = nullptr;
  /// Whether or not the dialog has been started.
  bool started = false;
  /// If not null, then is the active directory dialog running.
  std::unique_ptr<pfd::select_folder> dirDialog;
  /// If not null, then is the active file dialog running.
  std::unique_ptr<pfd::open_file> fileDialog;
  /// The ID for the file dialog.
  int fileDialogID = 0;
  /// The ID for the directory dialog.
  int dirDialogID = 0;
  /// Constructs a new instance of the implementation data.
  ///
  /// @param n A pointer to the dialog name.
  DialogImpl(const char* n) : name(n)
  {
  }
};

Dialog::Dialog(const char* n) : impl(new DialogImpl(n))
{

}

Dialog::~Dialog()
{
  delete impl;
}

bool Dialog::render(Editor* editor)
{
  if (!impl->started) {
    ImGui::OpenPopup(impl->name);
    impl->started = true;
  }

  checkFileSelect();
  checkPathSelect();

  if (impl->fileDialog || impl->dirDialog) {
    return true;
  }

  if (ImGui::BeginPopupModal(impl->name, nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
    renderInner(editor);
    ImGui::EndPopup();
    return true;
  } else {
    return false;
  }
}

bool Dialog::checkFileSelect()
{
  if (!impl->fileDialog) {
    return false;
  }

  if (!impl->fileDialog->ready()) {
    return false;
  }

  auto tmp = impl->fileDialog->result();

  if (!tmp.empty() && !tmp[0].empty()) {
    onSelectFile(impl->fileDialogID, tmp[0].c_str());
  }

  impl->fileDialog.reset();
  impl->fileDialogID = 0;

  return true;
}

bool Dialog::checkPathSelect()
{
  if (!impl->dirDialog) {
    return false;
  }

  if (!impl->dirDialog->ready()) {
    return false;
  }

  auto tmp = impl->dirDialog->result();

  if (!tmp.empty()) {
    onSelectPath(impl->dirDialogID, tmp.c_str());
  }

  impl->dirDialog.reset();
  impl->dirDialogID = 0;

  return true;
}

void Dialog::close()
{
  ImGui::CloseCurrentPopup();
}

void Dialog::selectFile(int id, const char* name)
{
  if (impl->fileDialog) {
    return;
  }

  impl->fileDialogID = id;

  impl->fileDialog.reset(new pfd::open_file(name));
}

void Dialog::selectPath(int id, const char* name)
{
  if (impl->dirDialog) {
    return;
  }

  impl->dirDialogID = id;

  impl->dirDialog.reset(new pfd::select_folder(name));
}

} // namespace px
