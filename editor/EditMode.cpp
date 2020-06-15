#include "EditMode.hpp"

#include "Editor.hpp"
#include "Mode.hpp"

namespace px {

namespace {

class EditMode final : public Mode
{
public:
  EditMode(Editor*) {}
  void leftClick(bool) override
  {
  }
  void rightClick(bool) override
  {
  }
  void mouseMotion(unsigned, unsigned) override
  {
  }
  void render() override
  {
  }
};

} // namespace

Mode* createEditMode(Editor* e)
{
  return new EditMode(e);
}

} // namespace px
