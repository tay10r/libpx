#ifndef LIBPX_EDITOR_DIALOG_HPP
#define LIBPX_EDITOR_DIALOG_HPP

namespace px {

class Editor;

/// The base for any sort of dialog.
class Dialog
{
  /// A pointer to the dialog name.
  const char* name = nullptr;
  /// Whether or not the dialog has been started.
  bool started = false;
public:
  /// Constructs a new base dialog instance.
  ///
  /// @param n The name to give the dialog.
  Dialog(const char* n) : name(n), started(false) {}
  /// Just a stub.
  virtual ~Dialog() {}
  /// Renders the dialog.
  ///
  /// @param editor A pointer to the editor instance
  /// that's calling the dialog.
  ///
  /// @return If false is returned, then the
  /// dialog should be deleted.
  bool render(Editor* editor);
protected:
  /// Renders the inner portion of the dialog.
  virtual void renderInner(Editor* editor) = 0;
  /// Closes the dialog.
  void close();
};

} // namespace

#endif // LIBPX_EDITOR_DIALOG_HPP
