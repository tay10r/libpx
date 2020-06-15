#ifndef LIBPX_LIBPX_HPP
#define LIBPX_LIBPX_HPP

#include <memory>

namespace px {

struct Document;
struct Fill;
struct Image;
struct Line;

/// A type definition for a single byte.
using Byte = unsigned char;

/// A type definition for a scalar value
/// in image space. This is used for points,
/// colors, similar items.
using Scalar = int;

/// A type definition for a size value.
/// This is used for indices and array sizes.
using Size = unsigned long;

/// Creates a new image instance.
///
/// @param width The width to give the image, in pixels.
/// @param height The height to give the image, in pixels.
///
/// @return A pointer to a new image instance.
/// If a memory allocation fails, then a null
/// pointer is returned.
Image* createImage(Size width, Size height) noexcept;

/// Releases memory allocated by an image.
///
/// @param image A pointer to the image
/// returned by @ref createImage. This
/// parameter may be a null pointer.
void closeImage(Image* image) noexcept;

/// Accesses the color buffer of an image.
///
/// @param image The image to get the color buffer of.
///
/// @return A pointer to the image color buffer.
/// The colors are in the format RGBA.
const Byte* getColorBuffer(const Image* image) noexcept;

/// Accesses the width of an image.
///
/// @param image The image to get the width of.
///
/// @return The width of the image, in pixels.
Size getImageWidth(const Image* image) noexcept;

/// Accesses the height of an image.
///
/// @param image The image to get the height of.
///
/// @return The height of the image, in pixels.
Size getImageHeight(const Image* image) noexcept;

/// Resizes an image.
///
/// @param image A pointer to an image returned by @ref createImage.
/// @param width The new width to assign the image.
/// @param height The new height to assign the image.
///
/// @return True on success, false on failure.
/// This function may fail if a memory allocation fails.
bool resizeImage(Image* image, Size width, Size height) noexcept;

/// Creates a new document instance.
///
/// @return A new document instance.
/// If a memory allocation fails, then
/// a null pointer is returned.
Document* createDoc() noexcept;

/// Releases memory allocated by a document.
///
/// @param doc The document to release the memory of.
void closeDoc(Document* doc) noexcept;

/// Adds a line to a document.
///
/// @return A pointer to a new line instance.
/// The line is owned by the document and does
/// not need to be manually destroyed. If a memory
/// allocation fails, then a null pointer is returned.
Line* addLine(Document* doc) noexcept;

/// Adds a fill operation to the document.
///
/// @param doc The document to add the fill operation to.
///
/// @return A pointer to a new fill operation.
Fill* addFill(Document* doc) noexcept;

/// Gets the width of the document, in pixels.
///
/// @param doc The document to get the width of.
///
/// @return The width of the document.
Size getDocWidth(const Document* doc) noexcept;

/// Gets the height of the document, in pixels.
///
/// @param doc The document to get the height of.
///
/// @return The height of the document.
Size getDocHeight(const Document* doc) noexcept;

/// Resizes the document.
/// Internally, all this does is change the
/// width and height parameters of the document.
/// This function does not perform image resizing.
///
/// @param doc The document to change the size of.
/// @param width The width to assign the document, in terms of pixels.
/// @param height The height to assign the document, in terms of pixels.
void resizeDoc(Document* doc, Size width, Size height) noexcept;

/// Sets the background color of a document.
void setBackground(Document* doc,
                   Byte r,
                   Byte g,
                   Byte b,
                   Byte a) noexcept;

/// Sets the origin of a fill operation.
///
/// @param fill The fill operation to set the origin of.
/// @param x The X position of the origin.
/// @param y The Y position of the origin.
void setFillOrigin(Fill* fill, Scalar x, Scalar y) noexcept;

/// Sets the color of the fill operation.
///
/// @param fill The fill operation to set the color of.
/// @param r The red channel value of the fill operation (0 to 255)
/// @param g The green channel value of the fill operation (0 to 255)
/// @param b The blue channel value of the fill operation (0 to 255)
/// @param a The alpha channel value of the fill operation (0 to 255)
void setFillColor(Fill* fill, Byte r, Byte g, Byte b, Byte a) noexcept;

/// Adds a point to a line.
///
/// @param line The line to add the point to.
/// @param x The X coordinate of the point to add.
/// @param y The Y coordinate of the point to add.
///
/// @return True on success, false on failure.
/// This function may fail if a memory allocation fails.
bool addPoint(Line* line, Scalar x, Scalar y) noexcept;

/// Sets the color of a line.
///
/// @param line The line to set the color of.
/// A new line can be created by calling @ref addLine
void setLineColor(Line* line, Byte r, Byte g, Byte b, Byte a) noexcept;

/// Sets the pixel size of a line.
///
/// @param line The line to set the pixel size of.
/// A new line can be created by calling @ref addLine
///
/// @param pixelSize The pixel size to assign. This
/// acts as both the width and height of the pixel,
/// since all pixels are squares.
void setPixelSize(Line* line, Scalar pixelSize) noexcept;

/// Renders the document onto a color buffer.
///
/// @param doc The document to be rendered.
/// @param color The color buffer to render to.
/// @param w The width of the color buffer.
/// @param h The height of the color buffer.
void render(const Document* doc, Byte* color, Size w, Size h) noexcept;

/// Renders the document onto an instance of @ref Image.
///
/// @param doc the document to be rendered.
/// @param image The image to render the document onto.
void render(const Document* doc, Image* image) noexcept;

} // namespace px

#endif /* LIBPX_LIBPX_HPP */
