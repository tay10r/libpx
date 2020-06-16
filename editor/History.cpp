#include "History.hpp"

#include <libpx.hpp>

#include <vector>

namespace px {

class HistoryImpl final
{
  friend History;

  std::vector<Document*> snapshots;
  std::size_t pos = 0;

  ~HistoryImpl();
};

History::History() : impl(new HistoryImpl())
{
  impl->snapshots.emplace_back(createDoc());
}

History::~History()
{
  delete impl;
}

Document* History::getDocument() noexcept
{
  return impl->snapshots[impl->pos];
}

const Document* History::getDocument() const noexcept
{
  return impl->snapshots[impl->pos];
}

void History::snapshot()
{
  impl->snapshots.resize(impl->pos + 1);

  impl->snapshots.emplace_back(copyDoc(getDocument()));

  impl->pos = impl->snapshots.size() - 1;
}

void History::undo()
{
  if (impl->pos > 0) {
    impl->pos--;
  }
}

void History::redo()
{
  if ((impl->pos + 1) < impl->snapshots.size()) {
    impl->pos++;
  }
}

HistoryImpl::~HistoryImpl()
{
  for (auto* doc : snapshots) {
    closeDoc(doc);
  }
};

} // namespace px
