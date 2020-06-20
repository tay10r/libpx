#ifndef LIBPX_EDITOR_LOG_HPP
#define LIBPX_EDITOR_LOG_HPP

#include <cstddef>

namespace px {

class LogImpl;

/// A widget used for logging information.
class Log final {
  /// A pointer to the implementation data.
  LogImpl* self = nullptr;
public:
  /// Constructs a new instance of a log.
  Log();
  /// Releases memory allocated by the log.
  ~Log();
  /// Renders a frame of the log widget.
  void frame();
  /// Logs an error message.
  template <typename... Args>
  void logError(Args... args)
  {
    if (canWrite()) {
      writeTime();
      log(" Error: ", args..., '\n');
    }
  }
protected:
  // Deleted to prevent misuse.
  Log(const Log&) = delete;
  /// Expands an argument list to get written to the log.
  template <typename T, typename... Args>
  void log(const T& first, const Args&... others)
  {
    write(first);

    log(others...);
  }
  /// Terminates the log expansion.
  template <typename Last>
  void log(const Last& last)
  {
    write(last);
  }
  /// Writes the current time to the log.
  void writeTime();
  /// Writes a string to the log.
  void write(const char* str);
  /// Writes a single character to the log.
  void write(char c);
  /// Writes size value to the log.
  void write(std::size_t n);
  /// Writes an integer to the log.
  void write(int n);
  /// Writes a floating point type to the log.
  void write(float n);
  /// Indicates if the log can be written to.
  bool canWrite();
};

} // namespace px

#endif // LIBPX_EDITOR_LOG_HPP
