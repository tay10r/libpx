#include "AppStorage.hpp"

#include <libpx.hpp>

#include <emscripten.h>

extern "C" {

void pxEditAppStorageSync(void* data, char* result)
{
  px::AppStorage::Observer* observer = (px::AppStorage::Observer*)data;

  if (observer) {
    observer->observeSyncResult(result);
  }
}

} // extern "C"

namespace px {

bool AppStorage::init(Observer* observer)
{
  EM_ASM({
    pxedit.setSyncCallback($0, "pxEditAppStorageSync");
    pxedit.initAppStorage();
  }, observer);

  return true;
}

AppStorage::Path AppStorage::getPrefix()
{
  return "/appStorage";
}

void AppStorage::syncToDevice(Observer* observer)
{
  EM_ASM({
    pxedit.setSyncCallback($0, "pxEditAppStorageSync");
    FS.syncfs(false, pxedit.syncResult);
  }, observer);
}

} // namespace px
