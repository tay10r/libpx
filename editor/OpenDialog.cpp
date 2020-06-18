#include "OpenDialog.hpp"

#include "Dialog.hpp"
#include "Editor.hpp"

#include <string>

namespace px {

namespace {

class OpenDialog final : public Dialog
{
  /// The path to the file that was selected.
  std::string docPath;
public:
  OpenDialog() : Dialog("")
  {
    selectFile(0, "Open an existing document.");
  }
  // Override to make sure we don't get an additional
  // ImGui window when we just want the native file dialog.
  bool render(Editor* editor) override
  {
    if (checkFileSelect()) {
      if (!docPath.empty()) {
        editor->openDoc(docPath.c_str());
      }
      return false;
    } else {
      return true;
    }
  }
protected:
  // Does nothing since we override 'render()'
  void renderInner(Editor*) override {}

  void onSelectFile(int, const char* path) override
  {
    docPath = path;
  }
};

} // namespace

Dialog* createOpenDialog()
{
  return new OpenDialog();
}

} // namespace px
