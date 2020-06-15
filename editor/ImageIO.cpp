#include "ImageIO.hpp"

#include "libpx.hpp"

#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "stb_image_write.h"

namespace px {

bool savePNG(const char* path, const Image* image)
{
  const Byte* color = getColorBuffer(image);
  auto w = getImageWidth(image);
  auto h = getImageHeight(image);

  return stbi_write_png(path, w, h, 4, color, w * 4);
}

} // namespace px
