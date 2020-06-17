#include "Fs.hpp"

namespace px {

namespace {

/// Gets the default native directory separator.
constexpr char dirSeparator() noexcept
{
#ifdef _WIN32
  return '\\';
#else
  return '/';
#endif
}

/// Indicates if a character is a directory separator.
constexpr bool isSeparator(char c) noexcept
{
  return (c == '/') || (c == '\\');
}

/// Indicates if a path as a trailing '\' or '/'
bool hasTrailingSeparator(const std::string& str) noexcept
{
  if (str.empty()) {
    return false;
  }

  return isSeparator(str[str.size() - 1]);
}

} // namespace

std::string combinePaths(const char* a, const char* b)
{
  std::string result;

  if (a) {
    result += a;
  }

  if (!result.empty() && !hasTrailingSeparator(result)) {
    result += dirSeparator();
  }

  if (b && isSeparator(b[0])) {
    result += (b + 1);
  } else {
    result += b;
  }

  return result;
}

} // namespace px
