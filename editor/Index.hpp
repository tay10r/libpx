#ifndef LIBPX_EDITOR_INDEX_HPP
#define LIBPX_EDITOR_INDEX_HPP

#include <cstddef>

namespace px {

struct Document;
struct ErrorList;

class IndexImpl;

/// Used to store information about
/// the documents in the application storage.
class Index final
{
  /// A pointer to the implementation data.
  IndexImpl* self = nullptr;
public:
  /// Represents one entry within the index.
  struct Entry final
  {
    /// The path to the document.
    const char* path = "";
    /// The name of the entry.
    const char* name = "";
    /// The Id of the entry.
    int id = 0;
    /// Whether or not this document has unsaved changes.
    bool unsaved = false;
  };
  /// Initializes a new instance of an index.
  Index();
  /// Initializes an index and opens a file
  /// containing index data.
  Index(const char* path);
  /// Moves an index from one variable to another.
  Index(Index&& other) noexcept
  {
    self = other.self;
    other.self = nullptr;
  }
  /// Copies an existing index.
  ///
  /// @param other The index to copy.
  Index(const Index& other);
  /// Releases memory allocated by the index.
  ~Index();
  /// Creates a new document.
  ///
  /// @return The ID of the document.
  int createDocument();
  /// Opens an index at a certain path.
  ///
  /// @param path The path to the index to open.
  ///
  /// @return True on success, false on failure.
  bool open(const char* path);
  /// Opens a document in the index.
  ///
  /// @param id The ID of the document to open.
  /// @param doc A pointer to the document instance to store the data into.
  /// @param errList Receives a list of errors if the document has errors opening.
  ///
  /// @return Zero on success, a copy of errno on failure.
  int openDocument(int id, Document* doc, ErrorList** errList);
  /// Removes a document, specified by its ID.
  ///
  /// @param id The ID of the document to remove.
  void removeDocument(int id);
  /// Removes the stash of a document.
  ///
  /// @param id The ID of the document to remove the stash of.
  void removeDocumentStash(int id);
  /// Renames a document.
  ///
  /// @param id The ID of the document to rename.
  /// @param name The name to give the document.
  void rename(int id, const char* name);
  /// Saves the index to a path.
  ///
  /// @param path The path to save the index at.
  ///
  /// @return True on success, false on failure.
  bool save(const char* path) const;
  /// Saves a document.
  ///
  /// @param id The ID of the document within the index.
  /// @param doc The document to save.
  ///
  /// @return True on success, false on failure.
  bool saveDocument(int id, const Document* doc);
  /// Changes whether or not a particular document has unsaved changes.
  ///
  /// @param id The ID of document to changed the state of.
  /// @param unsaved The unsaved state to assign the document.
  ///
  /// @return True on success, false on failure.
  bool setUnsaved(int id, bool unsaved);
  /// Saves a temporary copy of a document that contains unsaved changes.
  ///
  /// @param id The ID of the document to stash the data for.
  /// @param document The document instance containing the data to stash.
  ///
  /// @return True on success, false on failure.
  bool stashDocument(int id, const Document* document);
  /// Finds the entry to a document.
  ///
  /// @param id The ID to find the entry for.
  ///
  /// @return On success, the entry for the specified document.
  /// On failure, a default initialized entry instance.
  Entry findEntry(int id) const noexcept;
  /// Gets an entry within the index.
  ///
  /// @param index The index of the entry to get.
  ///
  /// @return The entry at the specified index.
  /// If the index is invalid, then a default initialized
  /// entry is returned instead.
  Entry getEntry(std::size_t index) const noexcept;
  /// Gets the number of entries in the index.
  ///
  /// @return The number of entries in the index.
  std::size_t getEntryCount() const noexcept;
protected:
  /// Indicates if an entry exists already
  /// for a given path.
  bool pathExists(const char* path) const noexcept;
};

} // namespace px

#endif // LIBPX_EDITOR_INDEX_HPP
