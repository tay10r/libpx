#include "DocumentProperties.hpp"

#include <imgui.h>
#include <imgui_stdlib.h>

#include <algorithm>
#include <string>

namespace px {

namespace {

/// The maximum value the width or height can take.
/// This is to prevent integer overflow during the rendering process.
/// It would probably be best to determine the actual maximum based
/// on the rendering algorithms at some point.
constexpr int maxSize() noexcept
{
  // This is the resolution limit of Photoshop.
  return 300000;
}

/// Clips a number between a minimum and maximum value.
///
/// @param n The value to clip.
///
/// @return The resultant value of @p n.
inline int clip(int n, int min, int max) noexcept
{
  return std::max(std::min(n, max), min);
}

class NullObserver final : public DocumentProperties::Observer
{
public:
  void observe(DocumentProperties::Event) override {}
};

} // namespace

class DocumentPropertiesImpl final
{
  friend DocumentProperties;
  /// The path to the directory that the document is saved at.
  std::string directoryPath;
  /// The name of the document.
  std::string name;
  /// The width of the document, in pixels.
  int width = 64;
  /// The height of the document, in pixels.
  int height = 64;
  /// Whether or not the document size is locked.
  bool sizeLock = true;
  /// The background color of the document.
  float backgroundColor[4] { 0, 0, 0, 0 };
};

DocumentProperties::DocumentProperties() : self(new DocumentPropertiesImpl()) { }

DocumentProperties::~DocumentProperties() { delete self; }

void DocumentProperties::frame(Observer* observer)
{
  NullObserver nullObserver;

  observer = observer ? observer : &nullObserver;

  ImGui::Begin("Document Properties", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

  int w = self->width;
  int h = self->height;
  bool sizeChanged = false;

  sizeChanged |= ImGui::InputInt("Width",  &w);
  sizeChanged |= ImGui::InputInt("Height", &h);

  if (sizeChanged && !self->sizeLock) {
    self->width  = clip(w, 1, maxSize());
    self->height = clip(h, 1, maxSize());
    observer->observe(Event::ChangeSize);
  }

  ImGui::Checkbox("Size Locked", &self->sizeLock);

  if (ImGui::ColorEdit4("Background Color", self->backgroundColor)) {
    observer->observe(Event::ChangeBackgroundColor);
  }

  if (ImGui::InputText("Directory", &self->directoryPath, 0 /* flags */, nullptr /* cb */, nullptr /* data */)) {
    observer->observe(Event::ChangeDirectory);
  }

  ImGui::SameLine();

  if (ImGui::Button("Browse")) {
    observer->observe(Event::ClickedDirectoryBrowse);
  }

  if (ImGui::InputText("Name", &self->name, 0 /* flags */, nullptr /* cb */, nullptr /* data */)) {
    observer->observe(Event::ChangeName);
  }

  ImGui::End();
}

std::size_t DocumentProperties::getWidth() const noexcept
{
  return std::size_t(self->width);
}

std::size_t DocumentProperties::getHeight() const noexcept
{
  return std::size_t(self->height);
}

} // namespace px
