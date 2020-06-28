#include "BrowseDocumentsState.hpp"

#include "App.hpp"
#include "AppStorage.hpp"
#include "DrawState.hpp"
#include "MenuBar.hpp"

#include <imgui.h>

#include <algorithm>
#include <string>
#include <vector>

namespace px {

namespace {

/// Contains information about a single
/// document in the document database.
struct Entry final
{
  /// The ID of the document.
  int id = 0;
  /// The name of the document.
  std::string name;
  /// The path to the document.
  std::string path;
  /// Whether or not the document has unsaved changes.
  bool unsaved = false;
  /// Whether or not this entry is currently selected.
  bool selected = false;

  // Other things to include here might
  // be the last modification date (for recent files)
  // or whether or not there's non-comitted changes.
};

/// Simple type definition for entries.
using EntryList = std::vector<Entry>;

/// Used for comparing entry names.
struct NameComparator final
{
  bool operator () (const Entry& a, const Entry& b) const noexcept
  {
    return a.name < b.name;
  }
};

/// Implements the documents browser interface.
class BrowseDocumentsStateImpl final : public BrowseDocumentsState,
                                       public AppStorage::Observer
{
  /// The entries in the documents database.
  EntryList entries;
public:
  /// Constructs a new instance of the documents browser.
  ///
  /// @param app A pointer to the hosting application.
  BrowseDocumentsStateImpl(App* app) : BrowseDocumentsState(app)
  {
    refresh();
  }
  /// Renders a frame of the document browser.
  void frame() override
  {
    ImGui::Begin("Open a Document");

    if (ImGui::BeginTable("Documents", 2)) {

      fillDocumentTable();

      ImGui::EndTable();
    }

    if (ImGui::Button("New")) {
      hitNew();
    }

    ImGui::SameLine();

    if (ImGui::Button("Delete")) {
      hitRemove();
    }

    ImGui::SameLine();

    if (ImGui::Button("Refresh")) {
      refresh();
    }

    ImGui::End();
  }
protected:
  void fillDocumentTable()
  {
    for (std::size_t i = 0; i < entries.size(); i++) {

      auto& entry = entries[i];

      ImGui::TableNextRow();

      ImGui::TableSetColumnIndex(0);

      if (ImGui::Selectable(entry.name.data(), &entry.selected)) {
        unselectAllExcept(i);
      }

      if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
        getApp()->openDocument(entry.id);
        setupDrawState();
      }

      ImGui::TableSetColumnIndex(1);

      if (entry.unsaved) {
        ImGui::BulletText("Unsaved Changes");
      } else {
        ImGui::Text("");
      }
    }
  }
  /// Called when the 'New' button is hit.
  void hitNew()
  {
    getApp()->createDocument();

    setupDrawState();
  }
  /// Called when the 'Remove' button is hit.
  void hitRemove()
  {
    std::size_t index = 0;

    if (!getSelected(index)) {
      return;
    }

    getApp()->removeDocument(entries[index].id);

    refresh();
  }
  /// Sets up the draw state and pushes it to
  /// the application. After calling this function,
  /// this application state will not be shown until
  /// the document is closed.
  void setupDrawState()
  {
    auto* app = getApp();

    app->pushAppState(DrawState::init(app));
  }
  /// Gets the currently selected item.
  ///
  /// @return True if the item was found,
  /// false if there is no items selected.
  bool getSelected(std::size_t& index)
  {
    for (std::size_t i = 0; i < entries.size(); i++) {
      if (entries[i].selected) {
        index = i;
        return true;
      }
    }

    return false;
  }
  /// Unselected all items except one.
  void unselectAllExcept(std::size_t& index)
  {
    for (std::size_t i = 0; i < entries.size(); i++) {
      entries[i].selected = (i == index);
    }
  }
  /// Updates the contents in the document browser.
  void refresh()
  {
    entries.clear();

    // TODO : May need to AppStorage::sync() here.

    AppStorage::listDocuments(this);

    std::sort(entries.begin(), entries.end(), NameComparator());
  }
  /// Observes a listed file.
  void observeListFile(int id, const char* path, const char* name, bool unsaved) override
  {
    Entry entry {
      id,
      name,
      path,
      unsaved
    };

    entries.emplace_back(entry);
  }
};

} // namespace

BrowseDocumentsState* BrowseDocumentsState::init(App* app)
{
  return new BrowseDocumentsStateImpl(app);
}

} // namespace px
