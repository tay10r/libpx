#include "ColorEdit.hpp"

namespace px {

bool ColorEdit4::operator () (const char* label, float* color, ImGuiColorEditFlags flags)
{
  if (committed) {
    committed = false;
  }

  auto modified = ImGui::ColorEdit4(label, color, flags);

  if (ImGui::IsItemActive()) {
    first = !active;
    active = true;
  } else if (active) {
    active = false;
    committed = true;
  }

  return modified;
}

} // namespace px
