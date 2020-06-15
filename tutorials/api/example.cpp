#include "libpx.hpp"

#include <cstdio>

bool savePPM(const char* filename, unsigned w, unsigned h, const unsigned char* data);

int main()
{
  unsigned width = 640;
  unsigned height = 480;

  auto* doc = px::createDoc();

  auto* l1 = px::addLine(doc);
  px::addPoint(l1, 25, 25);
  px::addPoint(l1, 75, 75);
  px::setPixelSize(l1, 4);

  auto* img = px::createImage();

  px::resizeImage(img, width, height);

  px::render(doc, img);

  px::closeDoc(doc);

  savePPM("example.ppm", width, height, px::getColorBuffer(img));

  px::closeImage(img);

  return 0;
}

bool savePPM(const char* filename, unsigned w, unsigned h, const unsigned char* data)
{
  FILE* file = std::fopen(filename, "wb");
  if (!file) {
    return false;
  }

  fprintf(file, "P6\n%u %u\n255\n", w, h);

  for (unsigned y = 0; y < h; y++) {
    for (unsigned x = 0; x < w; x++) {
      fwrite(data + (((y * w) + x) * 4), 3, 1, file);
    }
  }

  fclose(file);

  return true;
}
