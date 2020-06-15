#ifndef LIBPX_EDITOR_IMAGE_IO_HPP
#define LIBPX_EDITOR_IMAGE_IO_HPP

namespace px {

struct Image;

/// Saves an image to a PNG file.
///
/// @param path The path to save the file at.
/// @param image The image instance to be saved.
///
/// @return True on success, false on failure.
bool savePNG(const char* path, const Image* image);

} // namespace px

#endif // LIBPX_EDITOR_IMAGE_IO_HPP
