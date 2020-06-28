#include "AppStorage.hpp"

#include "Index.hpp"

#include <libpx.hpp>

#include <filesystem>
#include <sstream>

#include <cerrno>

namespace px {

int AppStorage::createDocument()
{
  Index index(getIndexPath().c_str());

  auto id = index.createDocument();

  return index.save(getIndexPath().c_str()) ? id : -1;
}

void AppStorage::removeDocument(int id)
{
  Index index(getIndexPath().c_str());

  index.removeDocument(id);

  index.save(getIndexPath().c_str());
}

void AppStorage::removeDocumentStash(int id)
{
  Index index(getIndexPath().c_str());

  index.removeDocumentStash(id);

  index.save(getIndexPath().c_str());
}

AppStorage::Path AppStorage::getDocumentPrefix()
{
  return std::filesystem::path(AppStorage::getPrefix()) / "Documents";
}

AppStorage::Path AppStorage::getIndexPath()
{
  return getPrefix() / "index.json";
}

std::string AppStorage::getDocumentName(int id)
{
  Index index(getIndexPath().c_str());

  auto entry = index.findEntry(id);

  return entry.name;
}

void AppStorage::renameDocument(int id, const char* name)
{
  Index index(getIndexPath().c_str());

  index.rename(id, name);

  index.save(getIndexPath().c_str());
}

int AppStorage::openDocument(int id, Document* doc, ErrorList** errList)
{
  Index index(getIndexPath().c_str());

  return index.openDocument(id, doc, errList);
}

bool AppStorage::saveDocument(int id, const Document* document)
{
  Index index(getIndexPath().c_str());

  if (!index.saveDocument(id, document)) {
    return false;
  }

  index.save(getIndexPath().c_str());

  return true;
}

bool AppStorage::setUnsaved(int id, bool unsaved)
{
  Index index(getIndexPath().c_str());

  index.setUnsaved(id, unsaved);

  return index.save(getIndexPath().c_str());
}

bool AppStorage::stashDocument(int id, const Document* document)
{
  Index index(getIndexPath().c_str());

  if (!index.stashDocument(id, document)) {
    return false;
  }

  return index.save(getIndexPath().c_str());
}

void AppStorage::listDocuments(Observer* observer)
{
  Index index(getIndexPath().c_str());

  for (std::size_t i = 0; i < index.getEntryCount(); i++) {

    auto entry = index.getEntry(i);

    observer->observeListFile(entry.id, entry.path, entry.name, entry.unsaved);
  }
}

} // namespace px
