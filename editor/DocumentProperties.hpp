#ifndef LIBPX_EDITOR_DOCUMENT_PROPERTIES_HPP
#define LIBPX_EDITOR_DOCUMENT_PROPERTIES_HPP

#include <cstddef>

namespace px {

struct Document;

/// Contains the implementation data
/// of the @ref DocumentProperties class.
class DocumentPropertiesImpl;

/// This is the widget that shows the document
/// properties to the user.
class DocumentProperties final
{
  /// Contains the implementation data.
  DocumentPropertiesImpl* self = nullptr;
public:
  /// Enumerates the events that can occur
  /// from the document properties widget.
  enum class Event
  {
    ChangeBackgroundColor,
    ChangeSize
  };
  /// Used for observing events
  /// that can occur from the document
  /// properties widget.
  class Observer
  {
  public:
    /// Just a stub.
    virtual ~Observer() {}
    /// Observes an event from the widget.
    virtual void observe(Event) = 0;
    /// Observes a document being renamed.
    ///
    /// @param name The name that the document was given.
    virtual void observeDocumentRename(const char* name)
    {
      (void)name;
    }
  };
  /// Constructs a new instance of the document properties class.
  DocumentProperties();
  /// Releases memory allocated by this class.
  ~DocumentProperties();
  /// Renders the document properties widget.
  void frame(Observer* observer = nullptr);
  /// Gets the width of the document.
  std::size_t getWidth() const noexcept;
  /// Gets the height of the document.
  std::size_t getHeight() const noexcept;
  /// Gets the background color of the document.
  const float* getBackgroundColor() const noexcept;
  /// Gets the current document name.
  ///
  /// @return A pointer to the current document name.
  const char* getDocumentName() const noexcept;
  /// Sets the name of the current document.
  ///
  /// @param name The name to assign the document.
  void setDocumentName(const char* name);
  /// Synchronizes information in the panel
  /// with the information from a new document.
  void sync(const Document* doc);

  DocumentProperties(const DocumentProperties&) = delete;
};

} // namespace px

#endif // LIBPX_EDITOR_DOCUMENT_PROPERTIES_HPP
