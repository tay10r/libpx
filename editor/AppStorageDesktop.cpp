#include "AppStorage.hpp"

#include <platform_folders.h>

#include <filesystem>

namespace px {

bool AppStorage::init(Observer* observer)
{
  auto prefix = std::filesystem::path(AppStorage::getPrefix());

  std::filesystem::create_directory(prefix);
  std::filesystem::create_directory(prefix / "Documents");

  observer->observeSyncResult(nullptr);

  return true;
}

AppStorage::Path AppStorage::getPrefix()
{
  return std::filesystem::path(sago::getDataHome()) / "pxedit";
}

void AppStorage::syncToDevice(Observer* observer)
{
  observer->observeSyncResult(nullptr);
}

} // namespace px
