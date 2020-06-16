#include "ImageIO.hpp"

#include "libpx.hpp"

#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "stb_image_write.h"

#include <vector>

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

} // namespace

bool savePNG(const char* path, const Image* image)
{
  auto w = getImageWidth(image);
  auto h = getImageHeight(image);

  auto pixels = toRGBA(image);

  return stbi_write_png(path, w, h, 4, pixels.data(), w * 4);
}

} // namespace px
