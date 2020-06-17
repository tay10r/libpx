#ifndef LIBPX_EDITOR_DIALOG_HPP
#define LIBPX_EDITOR_DIALOG_HPP

namespace px {

class Editor;

class DialogImpl;

/// The base for any sort of dialog.
class Dialog
{
  /// Contains the implementation of the base class.
  DialogImpl* impl = nullptr;
public:
  /// Constructs a new base dialog instance.
  ///
  /// @param n The name to give the dialog.
  Dialog(const char* n);
  /// Releases memory allocated by the dialog.
  virtual ~Dialog();
  /// Renders the dialog.
  ///
  /// @param editor A pointer to the editor instance
  /// that's calling the dialog.
  ///
  /// @return If false is returned, then the
  /// dialog should be deleted.
  virtual bool render(Editor* editor);
protected:
  /// Checks if a file has been selected from the native file dialog.
  bool checkFileSelect();
  /// Checks if a directory has been selected from the native file dialog.
  bool checkPathSelect();
  /// Can be called by derived classes
  /// to have the user select a directory path.
  ///
  /// @param id The Id supplied by the derived
  /// class to differentiate multiple requests.
  void selectPath(int id, const char* windowName);
  /// Can be called by derived classes
  /// to have the user select a file path.
  ///
  /// @param id The Id supplied by the derived
  /// class to differentiate multiple requests.
  void selectFile(int id, const char* windowName);

  // These two functions are passed the ID of a @ref selectFile
  // or @ref selectPath request as well as the paths that were selected.
  virtual void onSelectFile(int, const char*) {}
  virtual void onSelectPath(int, const char*) {}

  /// Renders the inner portion of the dialog.
  virtual void renderInner(Editor* editor) = 0;
  /// Closes the dialog.
  void close();
};

} // namespace

#endif // LIBPX_EDITOR_DIALOG_HPP
