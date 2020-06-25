#ifndef LIBPX_EDITOR_APP_STORAGE_HPP
#define LIBPX_EDITOR_APP_STORAGE_HPP

#include <filesystem>

namespace px {

struct Document;
struct ErrorList;

/// Application storage is where data
/// will be stored when it's not specifically
/// asked for by the user. This includes GUI
/// styling data, unsaved documents, saved documents
/// and document templates.
class AppStorage final
{
public:
  /// A type definition for a path.
  using Path = std::filesystem::path;
  /// Used for observing events from app storage.
  class Observer
  {
  public:
    /// This is called when a directory is listed
    /// and a single entry is read from the directory.
    ///
    /// @param id The ID of the document.
    /// @param path The path of the item.
    /// @param name The name of the item.
    /// @param unsaved Whether or not the document was unsaved at the last time it was edited.
    virtual void observeListFile(int id, const char* path, const char* name, bool unsaved)
    {
      (void)id;
      (void)name;
      (void)path;
      (void)unsaved;
    }
    /// Called when the result of a sync operation returns.
    ///
    /// @param errorMessage If not a null pointer, then it contains
    /// a string describing an error that occurred. If this parameter
    /// is a null pointer, then it indicates that the operation was successful.
    virtual void observeSyncResult(const char* errorMessage)
    {
      (void)errorMessage;
    }
  };
  /// Initialializes the app storage.
  /// This ensures that all directories
  /// for the application are found or created.
  ///
  /// @param observer Used to observe events that may
  /// happen during initialization.
  ///
  /// @return True on success, false on failure.
  static bool init(Observer* observer = nullptr);
  /// Creates a new document.
  ///
  /// @return The ID of the newly created document.
  /// A value that is less than zero indicates failure.
  static int createDocument();
  /// Gets the name of a document.
  ///
  /// @param id The ID to get the document name of.
  ///
  /// @return The name of the document.
  static std::string getDocumentName(int id);
  /// Opens an existing document.
  ///
  /// @param id The ID of the document to open.
  /// @param doc The document instance to receive the data.
  /// @param errList Receives an error list instance if opening the document fails.
  ///
  /// @return Zero on success, errno on failure.
  static int openDocument(int id, Document* doc, ErrorList** errList);
  /// Removes a document from application storage.
  ///
  /// @param id The ID of the document to remove.
  static void removeDocument(int id);
  /// Renames a document in the application storage.
  ///
  /// @param id The ID of the document to rename.
  /// @param The name to give the document.
  static void renameDocument(int id, const char* name);
  /// Saves a document to the application storage.
  ///
  /// @param id The ID of the document to save.
  /// @param document The document to save.
  ///
  /// @return True on success, false on failure.
  static bool saveDocument(int id, const Document* document);
  /// Sets whether or not a particular document has unsaved changes.
  ///
  /// @param id The ID of the document to change the unsaved state of.
  /// @param unsaved Whether or not this document has unsaved changes.
  ///
  /// @return True on success, false on failure.
  static bool setUnsaved(int id, bool unsaved);
  /// Stashes unsaved changes to application storage.
  ///
  /// @param id The ID of the document being stashed.
  /// This document will be marked as having unsaved changes
  /// after calling this function.
  ///
  /// @param document The document containing the data to stash.
  ///
  /// @return True on success, false on failure.
  static bool stashDocument(int id, const Document* document);
  /// Gets a path to the document index.
  ///
  /// @return A path to the document index.
  static Path getIndexPath();
  /// Lists the documents in app storage.
  ///
  /// @param observer The directory entries are passed to this observer.
  static void listDocuments(Observer* observer);
  /// Synchronizes the modifications made by the application
  /// to the data that persists on the storage device. This should
  /// be called everytime the file system is modified in app storage.
  static void syncToDevice(Observer* observer);
  /// Gets the prefix used to store files.
  /// For desktops, this depends on the platform.
  ///
  /// Ubuntu, for example, might be /home/john/.local/share/pxedit.
  ///
  /// For browsers, this is just "appStorage"
  static Path getPrefix();
  /// Gets the prefix used to find documents at.
  ///
  /// @return The prefix used to find documents at.
  static Path getDocumentPrefix();
protected:
  /// Generates a unique file name.
  ///
  /// @param prefix The prefix of the file name.
  /// @param ext The extension to add to the end of the file name.
  ///
  /// @return The resultant file name.
  static std::string getUniqueName(const char* prefix, const char* ext);
  /// Synchronizes operations done to storage
  /// to ensure that they're done at the point of this function call.
  ///
  /// @return True on success, false on failure.
  static bool sync();
};

} // namespace px

#endif // LIBPX_EDITOR_APP_STORAGE_HPP
