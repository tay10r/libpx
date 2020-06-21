#include "ImageIO.hpp"

#include <libpx.hpp>

#include "Blob.hpp"

#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <stb_image_write.h>

#include <vector>

#include <cstring>

namespace px {

namespace {

std::vector<unsigned char> toRGBA(const Image* image)
{
  auto w = getImageWidth(image);
  auto h = getImageHeight(image);

  std::vector<unsigned char> pixels(w * h * 4);

  const auto* src = getColorBuffer(image);

  for (std::size_t i = 0; i < (w * h * 4); i++) {
    pixels[i] = src[i] * 255;
  }

  return pixels;
}

/// Writes data to a blob.
/// This is used by the STB image library
/// when formatting a PNG file.
///
/// @param blobPtr A pointer to the blob.
/// @param data The data to write.
/// @param size The number of bytes to write.
void writeToBlob(void* blobPtr, void* data, int size)
{
  Blob* blob = (Blob*) blobPtr;

  auto writeLocation = blob->size();

  blob->expand(size);

  std::memcpy(blob->data() + writeLocation, data, size);
}

} // namespace

Blob formatPNG(const Image* image)
{
  Blob blob;

  auto w = getImageWidth(image);
  auto h = getImageHeight(image);

  auto pixels = toRGBA(image);

  stbi_write_png_to_func(&writeToBlob, &blob, w, h, 4, pixels.data(), w * 4);

  return blob;
}

bool savePNG(const char* path, const Image* image)
{
  auto w = getImageWidth(image);
  auto h = getImageHeight(image);

  auto pixels = toRGBA(image);

  return stbi_write_png(path, w, h, 4, pixels.data(), w * 4);
}

} // namespace px
