#ifndef LIBPX_EDITOR_FS_HPP
#define LIBPX_EDITOR_FS_HPP

#include <string>

namespace px {

/// Combines two paths into a new string.
///
/// @param a The left hand path operand.
/// @param b The right hand path operand.
/// @return The resultant path.
std::string combinePaths(const char* a, const char* b);

} // namespace px

#endif // LIBPX_EDITOR_FS_HPP
