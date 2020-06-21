#include "Fs.hpp"

#include <limits>
#include <sstream>

#include <cstring>

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

bool fileExists(const char* path)
{
  FILE* file = std::fopen(path, "rb");
  if (!file) {
    return false;
  }

  std::fclose(file);

  return true;
}

const char* getExtension(const char* path) noexcept
{
  std::size_t pathLength = std::strlen(path);

  for (std::size_t i = pathLength; i > 0; i--) {
    if (path[i - 1] == '.') {
      return path + i;
    }
  }

  return path + pathLength;
}

std::string removeExtension(const char* path)
{
  std::size_t pathLength = std::strlen(path);

  const char* ext = getExtension(path);

  if (ext == (path + pathLength)) {
    return path;
  }

  std::string out;

  for (std::size_t i = 0; (path + i) != (ext - 1); i++) {
    out += path[i];
  }

  return out;
}

std::string toUniquePath(const char* path)
{
  if (!fileExists(path)) {
    return path;
  }

  const char* ext = getExtension(path);

  std::string base = removeExtension(path);

  for (int i = 1; i < std::numeric_limits<int>::max(); i++) {

    std::stringstream pathStream;
    pathStream << base;
    pathStream << '(' << i << ')';

    if (ext && ext[0]) {
      pathStream << '.' << ext;
    }

    auto uniquePath = pathStream.str();
    if (!fileExists(uniquePath.c_str())) {
      return uniquePath;
    }
  }

  return "";
}

} // namespace px
