#ifndef LIBPX_EDITOR_BLOB_HPP
#define LIBPX_EDITOR_BLOB_HPP

#include <cstddef>

namespace px {

class BlobImpl;

/// Represents a chunk of binary data.
class Blob final
{
  /// A pointer to the implementation data.
  BlobImpl* self = nullptr;
public:
  /// Constructs an empty blob.
  Blob();
  /// Copies an existing blob.
  Blob(const Blob& other);
  /// Moves the blob from one variable to another.
  Blob(Blob&& other) noexcept;
  /// Releases memory allocated by the blob.
  ~Blob();
  /// Accesses a data pointer.
  unsigned char* data() noexcept;
  /// Accesses a constant data pointer.
  const unsigned char* data() const noexcept;
  /// Gets the number of bytes in the blob.
  std::size_t size() const noexcept;
  /// Expands the size of the blob by a certain number of bytes.
  void expand(std::size_t size);
  /// Resizes the blob to a certain number of bytes.
  void resize(std::size_t size);
  /// Accesses a specific byte within the blob.
  unsigned char& operator [] (std::size_t index) noexcept;
  /// Accesses a specific byte within the blob.
  const unsigned char& operator [] (std::size_t index) const noexcept;
  /// Assigns one blob to another, via copy.
  Blob& operator = (const Blob& other);
  /// Assigns one blob to another, via move.
  Blob& operator = (Blob&& other) noexcept;
};

} // namespace px

#endif // LIBPX_EDITOR_BLOB_HPP
