#include "libpx.hpp"

#include <cstdio>

bool savePPM(const char* filename, unsigned w, unsigned h, const float* data);

int main()
{
  unsigned width = 128;
  unsigned height = 128;

  auto* doc = px::createDoc();

  auto* l1 = px::addLine(doc);
  px::addPoint(l1, 32, 32);
  px::addPoint(l1, 96, 32);
  px::setPixelSize(l1, 4);

  auto* l2 = px::addLine(doc);
  px::addPoint(l2, 96, 32);
  px::addPoint(l2, 96, 96);
  px::setPixelSize(l2, 4);

  auto* l3 = px::addLine(doc);
  px::addPoint(l3, 96, 96);
  px::addPoint(l3, 32, 96);
  px::setPixelSize(l3, 4);

  auto* l4 = px::addLine(doc);
  px::addPoint(l4, 32, 96);
  px::addPoint(l4, 32, 32);
  px::setPixelSize(l4, 4);

  auto* fill = px::addFill(doc);
  px::setFillOrigin(fill, 64, 64);
  px::setColor(fill, 0.3, 0.4, 0.6);

  auto* img = px::createImage(128, 128);

  px::resizeImage(img, width, height);

  px::render(doc, img);

  px::closeDoc(doc);

  savePPM("example.ppm", width, height, px::getColorBuffer(img));

  px::closeImage(img);

  return 0;
}

bool savePPM(const char* filename, unsigned w, unsigned h, const float* data)
{
  FILE* file = std::fopen(filename, "wb");
  if (!file) {
    return false;
  }

  fprintf(file, "P6\n%u %u\n255\n", w, h);

  using Byte = unsigned char;

  for (unsigned y = 0; y < h; y++) {

    for (unsigned x = 0; x < w; x++) {

      const float* src = data + ((y * w) + x) * 4;

      Byte color[3] {
        Byte(src[0] * src[3] * 255),
        Byte(src[1] * src[3] * 255),
        Byte(src[2] * src[3] * 255)
      };

      fwrite(color, 3, 1, file);
    }
  }

  fclose(file);

  return true;
}
