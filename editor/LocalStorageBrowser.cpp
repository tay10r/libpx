#include "LocalStorage.hpp"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif /* __EMSCRIPTEN__ */

namespace px {

bool LocalStorage::save(const char* filename, const void* data, std::size_t size)
{
#ifdef __EMSCRIPTEN__
  EM_ASM_({ window.download($0, $1, $2) }, filename, data, size);
  return true;
#else
  (void)filename;
  (void)data;
  (void)size;
  return false;
#endif
}

} // namespace px
