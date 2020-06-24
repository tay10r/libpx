#include "LocalStorage.hpp"

#include <emscripten.h>

namespace px {

bool LocalStorage::save(const char* filename, const void* data, std::size_t size)
{
  EM_ASM_({ pxedit.download($0, $1, $2) }, filename, data, size);

  return true;
}

} // namespace px
