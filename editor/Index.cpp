#include "Index.hpp"

#include "AppStorage.hpp"

#include <libpx.hpp>

#include <filesystem>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <vector>

#include <cerrno>

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-local-typedef"
#endif

#include <nlohmann/json.hpp>

#ifdef __clang__
#pragma clang diagnostic pop
#endif

namespace px {

namespace {

/// Contains the data for a document entry within the index.
struct EntryImpl final
{
  /// The path to the document for this entry.
  std::string path;
  /// The name given to this entry.
  std::string name;
  /// The ID of this entry.
  int id = 0;
  /// Whether or not this entry has unsaved changes.
  bool unsaved = false;
};

/// Gets the path for a document state.
///
/// @param entry The entry to get the stash for.
///
/// @return The path to the document stash.
std::string getStashPath(const EntryImpl& entry)
{
  std::stringstream filenameStream;
  filenameStream << "document_";
  filenameStream << entry.id;
  filenameStream << "_stash.px";

  std::filesystem::path path(entry.path);

  path.replace_filename(filenameStream.str());

  return path.c_str();
}

} // namespace

/// Contains all the implementation data for the index.
class IndexImpl final
{
  friend Index;
  /// Contains information on all documents known to the index.
  std::vector<EntryImpl> entries;
  /// The value of the next document ID.
  int nextID = 0;
};

Index::Index() : self(new IndexImpl()) {}

Index::Index(const char* path) : Index()
{
  open(path);
}

Index::~Index() { delete self; }

int Index::createDocument()
{
  int id = self->nextID++;

  std::stringstream pathStream;
  pathStream << (AppStorage::getDocumentPrefix() / "document_").c_str();
  pathStream << id;
  pathStream << ".px";

  EntryImpl entry {
    pathStream.str(),
    "",
    id
  };

  self->entries.emplace_back(std::move(entry));

  return id;
}

void Index::removeDocument(int id)
{
  for (std::size_t i = 0; i < self->entries.size(); i++) {
    if (self->entries[i].id == id) {
      std::filesystem::remove(self->entries[i].path);
      self->entries.erase(self->entries.begin() + i);
    }
  }
}

void Index::rename(int id, const char* name)
{
  for (auto& ent : self->entries) {
    if (ent.id == id) {
      ent.name = name;
      return;
    }
  }
}

bool Index::open(const char* path)
{
  using namespace nlohmann;

  std::ifstream file(path);
  if (!file.good()) {
    return false;
  }

  self->entries.clear();

  json jsonRoot;

  file >> jsonRoot;

  auto jsonNextID = jsonRoot["next_id"];

  if (jsonNextID.is_number()) {
    self->nextID = jsonNextID.get<int>();
  } else {
    self->nextID = 0;
  }

  auto jsonDocs = jsonRoot["documents"];

  for (const auto& jsonDoc : jsonDocs) {

    EntryImpl entry {
      jsonDoc["path"].get<std::string>(),
      jsonDoc["name"].get<std::string>(),
      jsonDoc["id"].get<int>(),
      jsonDoc["unsaved"].get<bool>()
    };

    if (!pathExists(entry.path.c_str())) {
      self->entries.emplace_back(std::move(entry));
    }
  }

  return true;
}

int Index::openDocument(int id, Document* doc, ErrorList** errList)
{
  for (const auto& ent : self->entries) {
    if (ent.id != id) {
      continue;
    }

    std::string path = ent.unsaved ? getStashPath(ent) : ent.path;

    return openDoc(doc, path.c_str(), errList);
  }

  return ENOENT;
}

bool Index::save(const char* path) const
{
  using namespace nlohmann;

  std::ofstream file(path);
  if (!file.good()) {
    return false;
  }

  json jsonDocs;

  for (const auto& ent : self->entries) {

    json jsonDoc {
      { "path", ent.path },
      { "name", ent.name },
      { "id", ent.id },
      { "unsaved", ent.unsaved }
    };

    jsonDocs.push_back(jsonDoc);
  }

  json jsonRoot {
    { "documents", jsonDocs },
    { "next_id", self->nextID }
  };

  file << std::setw(4);

  file << jsonRoot;

  return true;
}

bool Index::saveDocument(int id, const Document* doc)
{
  const char* path = "";

  std::size_t entry_index = SIZE_MAX;

  for (std::size_t i = 0; i < self->entries.size(); i++) {
    if (self->entries[i].id == id) {
      entry_index = i;
      path = self->entries[i].path.c_str();
      break;
    }
  }

  if (entry_index == SIZE_MAX) {
    return false;
  }

  if (!saveDoc(doc, path)) {
    return false;
  }

  self->entries[entry_index].unsaved = false;

  return true;
}

bool Index::setUnsaved(int id, bool unsaved)
{
  for (auto& ent : self->entries) {
    if (ent.id == id) {
      ent.unsaved = unsaved;
      return true;
    }
  }

  return false;
}

bool Index::stashDocument(int id, const Document* doc)
{
  for (auto& ent : self->entries) {

    if (ent.id == id) {

      ent.unsaved = true;

      std::string path = getStashPath(ent);

      return saveDoc(doc, path.c_str());
    }
  }

  return false;
}

Index::Entry Index::findEntry(int id) const noexcept
{
  for (const auto& ent : self->entries) {
    if (ent.id == id) {
      return Entry {
        ent.path.c_str(),
        ent.name.c_str(),
        ent.id,
        ent.unsaved
      };
    }
  }

  return Entry {};
}

Index::Entry Index::getEntry(std::size_t index) const noexcept
{
  if (index >= self->entries.size()) {
    return Entry{};
  }

  return Entry {
    self->entries[index].path.c_str(),
    self->entries[index].name.c_str(),
    self->entries[index].id,
    self->entries[index].unsaved
  };
}

std::size_t Index::getEntryCount() const noexcept
{
  return self->entries.size();
}

bool Index::pathExists(const char* path) const noexcept
{
  for (const auto& ent : self->entries) {
    if (ent.path == path) {
      return true;
    }
  }

  return false;
}

} // namesapce px
