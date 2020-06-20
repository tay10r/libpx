#ifndef LIBPX_EDITOR_DOCUMENT_PROPERTIES_HPP
#define LIBPX_EDITOR_DOCUMENT_PROPERTIES_HPP

#include <cstddef>

namespace px {

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
    ChangeDirectory,
    ChangeName,
    ChangeSize,
    ClickedDirectoryBrowse
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

  DocumentProperties(const DocumentProperties&) = delete;
};

} // namespace px

#endif // LIBPX_EDITOR_DOCUMENT_PROPERTIES_HPP
