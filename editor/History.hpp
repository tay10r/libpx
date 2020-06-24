#ifndef LIBPX_EDITOR_HISTORY_HPP
#define LIBPX_EDITOR_HISTORY_HPP

namespace px {

struct Document;
struct ErrorList;

class HistoryImpl;

/// Used for storing snapshots of the
/// document for undo and redo operations.
class History final
{
  /// A pointer to the implementation data.
  HistoryImpl* impl = nullptr;
public:
  /// Constructs a new history instance,
  /// with an optional initial document.
  ///
  /// @param doc The initial document.
  /// This parameter is mainly useful
  /// when opening documents from the
  /// file system.
  History(Document* doc = nullptr);
  ~History();
  /// Gets a pointer to the current document.
  Document* getDocument() noexcept;
  /// Gets a pointer to the current document.
  const Document* getDocument() const noexcept;
  /// Opens a new document.
  ///
  /// @param path The path to open the document at.
  /// @param errList A pointer to the variable that
  /// receives the error list, if the open operation fails.
  ///
  /// @return Zero on success, errno on failure.
  int open(const char* path, ErrorList** errList);
  /// Marks the current document as the one that is saved.
  void markSaved();
  /// Indicates whether or not the current snapshot
  /// is the one that is saved.
  ///
  /// @return True if it is, false otherwise.
  bool isSaved() const noexcept;
  /// Pushes a new document to the history stack.
  /// If there were snapshots that were undone, they
  /// are erased at this point and no longer available for redo.
  void snapshot();
  /// Performs an undo operation.
  /// The next call to getDocument() will
  /// return a new pointer.
  void undo();
  /// Performs an undo operation.
  /// The next call to getDocument() will
  /// return a new pointer.
  void redo();
  /// Moves history data to a new variable.
  ///
  /// @param other The source history instance being moved.
  ///
  /// @return A reference to the destination history instance.
  History& operator = (History&& other) noexcept;
};

} // namespace px

#endif // LIBPX_EDITOR_HISTORY_HPP
