#include "LocalStorage.hpp"

#include "Fs.hpp"

#include <cstdio>

namespace px {

bool LocalStorage::save(const char* filename, const void* buf, std::size_t size)
{
  std::string path = toUniquePath(filename);

  FILE* file = std::fopen(path.c_str(), "wb");
  if (!file) {
    return false;
  }

  std::size_t writeSize = std::fwrite(buf, 1, size, file);

  std::fclose(file);

  return writeSize == size;
}

} // namespace px
