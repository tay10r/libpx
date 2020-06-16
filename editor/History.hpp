#ifndef LIBPX_EDITOR_HISTORY_HPP
#define LIBPX_EDITOR_HISTORY_HPP

namespace px {

class Document;
class HistoryImpl;

/// Used for storing snapshots of the
/// document for undo and redo operations.
class History final
{
  /// A pointer to the implementation data.
  HistoryImpl* impl = nullptr;
public:
  History();
  ~History();
  /// Gets a pointer to the current document.
  Document* getDocument() noexcept;
  /// Gets a pointer to the current document.
  const Document* getDocument() const noexcept;
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
};

} // namespace px

#endif // LIBPX_EDITOR_HISTORY_HPP
