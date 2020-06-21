#ifndef LIBPX_EDITOR_BUCKET_TOOL_HPP
#define LIBPX_EDITOR_BUCKET_TOOL_HPP

#include "DrawTool.hpp"

namespace px {

class BucketTool final : public DrawTool
{
public:
  using DrawTool::DrawTool;
protected:
  void onBegin(const MouseButtonEvent&, int, int) override;
};
} // namespace px

#endif // LIBPX_EDITOR_BUCKET_TOOL_HPP
