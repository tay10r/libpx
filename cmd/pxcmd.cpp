#include <libpx.hpp>

#include <string>
#include <vector>

#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace {

bool isNonOpt(const char* arg) noexcept
{
  return arg[0] != '-';
}

bool isOpt(const char* arg, const char* s, const char* l) noexcept
{
  return (std::strcmp(arg, s) == 0) || (std::strcmp(arg, l) == 0);
}

bool process(const char* filename)
{
  px::Document* doc = px::createDoc();

  px::ErrorList* errList = nullptr;

  int err = px::openDoc(doc, filename, &errList);
  if (err != 0) {
    if (errList) {
      px::printErrorListToStderr(errList);
      px::closeErrorList(errList);
    } else {
      std::fprintf(stderr, "Failed to open '%s' (%s)\n", filename, std::strerror(err));
    }
    px::closeDoc(doc);
    return false;
  }

  px::closeDoc(doc);

  return true;
}

} // namespace

int main(int argc, char** argv)
{
  std::vector<std::string> nonOpts;

  for (int i = 1; i < argc; i++) {
    if (isOpt(argv[i], "-h", "--help")) {
      std::fprintf(stderr, "Usage: %s [options] <files>\n", argv[0]);
      return EXIT_FAILURE;
    } else if (isNonOpt(argv[i])) {
      nonOpts.emplace_back(argv[i]);
    } else {
      std::fprintf(stderr, "Unknown option '%s'\n", argv[i]);
      return EXIT_FAILURE;
    }
  }

  if (nonOpts.empty()) {
    std::fprintf(stderr, "No files specified.\n");
    return EXIT_FAILURE;
  }

  auto success = true;

  for (const auto& filename : nonOpts) {
    success &= process(filename.c_str());
  }

  return success ? EXIT_SUCCESS : EXIT_FAILURE;
}
