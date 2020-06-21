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

/// Indicates if a file exists or not.
///
/// @param path The file to check for.
///
/// @return True if it exists, false if it does not.
bool fileExists(const char* path);

/// Gets the extension associated with a path.
///
/// @param path The path to get the extension of.
///
/// @return The extension of the path.
/// If the path has no extension, then a
/// pointer to the end of @p path is returned.
const char* getExtension(const char* path) noexcept;

/// Removes an extension from a path.
/// The '.' is not included in the resultant path.
///
/// @param path The path to remove the extension of.
///
/// @return The path @p path without the extension.
std::string removeExtension(const char* path);

/// Creates a new path from @p path that will
/// not cause the file at @p path to be overwritten.
///
/// @param path The base path to generate a unique
/// path from.
///
/// @return A unique variant of @p path.
/// If a unique path cannot be made (unlikely),
/// then an empty string is returned.
std::string toUniquePath(const char* path);

} // namespace px

#endif // LIBPX_EDITOR_FS_HPP
