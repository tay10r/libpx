#ifndef LIBPX_EDITOR_LOCAL_STORAGE_HPP
#define LIBPX_EDITOR_LOCAL_STORAGE_HPP

#include <cstddef>

namespace px {

/// This is an interface to the local file system.
/// It can have a very different underlying implementation
/// depending on whether the program is running natively or
/// running through a browser.
class LocalStorage
{
public:
  /// Saves a file to local storage.
  ///
  /// @param filename The name to give the file.
  /// @param data The buffer containing the file data.
  /// @param size The number of bytes in the buffer.
  ///
  /// @return True on success, false on failure.
  static bool save(const char* filename, const void* data, std::size_t size);
};

} // namespace px

#endif // LIBPX_EDITOR_LOCAL_STORAGE_HPP
