#include "Log.hpp"

#include <chrono>
#include <sstream>
#include <iomanip>

#include <ctime>

#include <imgui.h>

namespace px {

namespace {

/// The maximum amount of characters
/// that can go into the log before it
/// stops logging information.
constexpr std::size_t maxLogSize() noexcept
{
  // 128K max log size.
  return 128 * 1024;
}

} // namespace

/// Contains the implementation data of the log.
class LogImpl final
{
  friend Log;
  /// The stream that the info is going to.
  std::ostringstream stream;
  /// Indicates whether or not the log can be written to.
  bool canWrite()
  {
    auto p = stream.tellp();

    return p < decltype(p)(maxLogSize());
  }
};

Log::Log() : self(new LogImpl()) {}

Log::~Log() { delete self; }

void Log::copyToClipboard()
{
  auto text = self->stream.str();

  if (text.empty()) {
    ImGui::SetClipboardText("Log is empty.");
  } else {
    ImGui::SetClipboardText(text.c_str());
  }
}

void Log::frame()
{
  auto content = self->stream.str();

  ImGui::Begin("Log", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

  ImGui::InputTextMultiline("", &content[0], content.size(), ImVec2(0, 0), ImGuiInputTextFlags_ReadOnly);

  ImGui::End();
}

void Log::writeTime()
{
  auto now = std::chrono::system_clock::now();

  auto time = std::chrono::system_clock::to_time_t(now);

  self->stream << std::put_time(std::localtime(&time), "%T");
}

void Log::write(const char* str)
{
  self->stream << str;
}

void Log::write(char c)
{
  self->stream << c;
}

void Log::write(float n)
{
  self->stream << n;
}

void Log::write(int n)
{
  self->stream << n;
}

void Log::write(std::size_t n)
{
  self->stream << n;
}

bool Log::canWrite()
{
  return self->canWrite();
}

} // namespace px
