#include "PoseMode.hpp"

#include "Editor.hpp"
#include "Mode.hpp"

namespace px {

namespace {

class PoseMode final : public Mode
{
public:
  PoseMode(Editor*) {}
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

Mode* createPoseMode(Editor* e)
{
  return new PoseMode(e);
}

} // namespace px
