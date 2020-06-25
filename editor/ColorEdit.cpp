#include "ColorEdit.hpp"

namespace px {

bool ColorEdit4::operator () (const char* label, float* color, ImGuiColorEditFlags flags)
{
  if (committed) {
    committed = false;
  }

  auto change = ImGui::ColorEdit4(label, color, flags);
  if (change) {
    first = !changing;
    changing = true;
  }

  if (changing && !ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
    first = false;
    changing = false;
    committed = true;
  }

  return change;
}

} // namespace px
