/// @file libpx.hpp
///
/// @brief Contains all of the public API declarations
/// that are available to users.

/// @mainpage libpx Documentation
///
/// @section pxGettingStarted Getting Started
///
/// To get started using this library, the first
/// set of functions to use are one of:
///
/// - @ref px::createDoc
/// - @ref px::createImage
///
/// Checkout @ref pxDocumentApi and @ref pxImageApi for more
/// information on using those objects.
///
/// Here's a code example that demonstrates the initialization and cleanup routines.
/// @code{.cpp}
///
/// #include <libpx.hpp>
///
/// int main() {
///   /* The document gets rendered to this object. */
///   px::Image* img = px::createImage();
///   /* Used to describe what to render (geometry, color, etc.) */
///   px::Document* doc = px::createDoc();
///
///   /* Use library */
///
///   /* Finish using library and release memory */
///   px::closeDoc(doc);
///   px::closeImage(img);
///   return 0;
/// }
/// @endcode
///
/// @section pxAddingContent Adding Content
///
/// Once an instance of @ref px::Document has been created,
/// content can be added to it so that we have something to render.
///
/// @section pxRenderingContent Rendering Content
///
/// After content has been added to our document,
/// we can render it onto an instance of @ref px::Image.
///
/// This can be done using one of the @ref px::render functions.

#ifndef LIBPX_LIBPX_HPP
#define LIBPX_LIBPX_HPP

#include <cstddef>

/// @brief All declarations for this
/// library are put into this namespace.
namespace px {

struct Document;
struct Ellipse;
struct Fill;
struct Image;
struct Line;
struct Quad;

/// @defgroup pxDocumentApi Document API
///
/// @brief Contains all declarations related to the document object.

/// @defgroup pxImageApi Image API
///
/// @brief Contains all declarations related to the image API.

/// Creates a new image instance.
///
/// @exception std::bad_alloc If the image allocation fails
/// or if the color buffer allocation fails.
///
/// @param width The width to give the image, in pixels.
/// @param height The height to give the image, in pixels.
///
/// @return A pointer to a new image instance.
///
/// @ingroup pxImageApi
Image* createImage(std::size_t width, std::size_t height);

/// Releases memory allocated by an image.
///
/// @param image A pointer to the image
/// returned by @ref createImage. This
/// parameter may be a null pointer.
///
/// @ingroup pxImageApi
void closeImage(Image* image) noexcept;

/// Accesses the color buffer of an image.
///
/// @param image The image to get the color buffer of.
///
/// @return A pointer to the image color buffer.
/// The colors are in the format RGBA. The RGB
/// components are premultiplied.
///
/// @ingroup pxImageApi
const float* getColorBuffer(const Image* image) noexcept;

/// Accesses the width of an image.
///
/// @param image The image to get the width of.
///
/// @return The width of the image, in pixels.
///
/// @ingroup pxImageApi
std::size_t getImageWidth(const Image* image) noexcept;

/// Accesses the height of an image.
///
/// @param image The image to get the height of.
///
/// @return The height of the image, in pixels.
///
/// @ingroup pxImageApi
std::size_t getImageHeight(const Image* image) noexcept;

/// Resizes an image.
///
/// @exception std::bad_alloc If the color buffer resize fails.
///
/// @param image A pointer to an image returned by @ref createImage.
/// @param width The new width to assign the image.
/// @param height The new height to assign the image.
///
/// @ingroup pxImageApi
void resizeImage(Image* image, std::size_t width, std::size_t height);

/// Creates a new document instance.
///
/// @return A new document instance.
///
/// @ingroup pxDocumentApi
Document* createDoc();

/// Releases memory allocated by a document.
///
/// @param doc The document to release the memory of.
///
/// @ingroup pxDocumentApi
void closeDoc(Document* doc) noexcept;

/// Adds a line to a document.
///
/// @return A pointer to a new line instance.
/// The line is owned by the document and does
/// not need to be manually destroyed.
///
/// @ingroup pxDocumentApi
Line* addLine(Document* doc);

/// Adds an ellipse to a document.
///
/// @exception std::bad_alloc If the ellipse allocation fails
/// or if the the ellipse array resize fails.
///
/// @param doc The document to add the ellipse to.
///
/// @return A pointer to the new ellipse instance.
Ellipse* addEllipse(Document* doc);

/// Adds a fill operation to the document.
///
/// @param doc The document to add the fill operation to.
///
/// @return A pointer to a new fill operation.
///
/// @ingroup pxDocumentApi
Fill* addFill(Document* doc);

/// Adds a quadrilateral to the document.
///
/// @param doc The document to add the quadrilateral to.
///
/// @return A pointer to the new quad structure.
///
/// @ingroup pxDocumentApi
Quad* addQuad(Document* doc);

/// Gets the width of the document, in pixels.
///
/// @param doc The document to get the width of.
///
/// @return The width of the document.
///
/// @ingroup pxDocumentApi
std::size_t getDocWidth(const Document* doc) noexcept;

/// Gets the height of the document, in pixels.
///
/// @param doc The document to get the height of.
///
/// @return The height of the document.
///
/// @ingroup pxDocumentApi
std::size_t getDocHeight(const Document* doc) noexcept;

/// Resizes the document.
/// Internally, all this does is change the
/// width and height parameters of the document.
/// This function does not perform image resizing.
///
/// @param doc The document to change the size of.
/// @param width The width to assign the document, in terms of pixels.
/// @param height The height to assign the document, in terms of pixels.
///
/// @ingroup pxDocumentApi
void resizeDoc(Document* doc, std::size_t width, std::size_t height) noexcept;

/// Sets the background color of a document.
///
/// @ingroup pxDocumentApi
void setBackground(Document* doc, float r, float g, float b, float a) noexcept;

/// @defgroup pxEllipseApi Ellipse API
///
/// @brief Contains all declarations for ellipses.

/// Sets the color of an ellipse.
///
/// @param ellipse The ellipse to set the color of.
///
/// @ingroup pxEllipseApi
void setColor(Ellipse* ellipse, float r, float g, float b) noexcept;

/// Sets the center point of an ellipse.
///
/// @param ellipse The ellipse to set the enter of.
/// @param x The X coordinate to assign the center.
/// @param y The Y coordinate to assign the center.
void setCenter(Ellipse* ellipse, int x, int y) noexcept;

/// Sets the radius of an ellipse.
///
/// @param ellipse The ellipse to set the radius of.
/// @param x The X component of the radius.
/// @param y The Y component of the radius.
void setRadius(Ellipse* ellipse, int x, int y) noexcept;

/// Sets the pixel size of an ellipse.
///
/// @param ellipse The ellipse to set the pixel size of.
///
/// @param pixelSize The pixel size to assign the ellipse.
void setPixelSize(Ellipse* ellipse, int pixelSize) noexcept;

/// Resizes an ellipse to fill a rectangle.
///
/// @param ellipse The ellipse to resize.
/// @param x1 The X coordinate of the first point.
/// @param y1 The Y coordinate of the first point.
/// @param x2 The X coordinate of the second point.
/// @param y2 The Y coordinate of the second point.
void resizeRect(Ellipse* ellipse, int x1, int y1, int x2, int y2) noexcept;

/// @defgroup pxFillApi Flood Fill API
///
/// @brief Contains all declarations for flood fills.

/// Sets the origin of a fill operation.
///
/// @param fill The fill operation to set the origin of.
/// @param x The X position of the origin.
/// @param y The Y position of the origin.
///
/// @ingroup pxFillApi
void setFillOrigin(Fill* fill, int x, int y) noexcept;

/// Sets the color of the fill operation.
///
/// @param fill The fill operation to set the color of.
/// @param r The red channel value of the fill operation (0 to 1)
/// @param g The green channel value of the fill operation (0 to 1)
/// @param b The blue channel value of the fill operation (0 to 1)
///
/// @ingroup pxFillApi
void setColor(Fill* fill, float r, float g, float b) noexcept;

/// @defgroup pxLineApi Line API
///
/// @brief Contains all declarations for lines.

/// Sets the color of a line.
///
/// @param line The line to set the color of.
/// A new line can be created by calling @ref addLine
///
/// @ingroup pxLineApi
void setColor(Line* line, float r, float g, float b) noexcept;

/// Adds a point to a line.
///
/// @param line The line to add the point to.
/// @param x The X coordinate of the point to add.
/// @param y The Y coordinate of the point to add.
///
/// @ingroup pxLineApi
void addPoint(Line* line, int x, int y);

/// Sets the pixel size of a line.
///
/// @param line The line to set the pixel size of.
/// A new line can be created by calling @ref addLine
///
/// @param pixelSize The pixel size to assign. This
/// acts as both the width and height of the pixel,
/// since all pixels are squares.
///
/// @ingroup pxLineApi
void setPixelSize(Line* line, int pixelSize) noexcept;

/// Sets the position of an existing point in the line.
///
/// @param line The line to modify the point of.
/// @param index The index of the point to modify.
/// @param x The X coordinate to assign the point.
/// @param y The Y coordinate to assign the point.
///
/// @return True on success, false on failure.
/// This function returns false of @p index is out of bounds.
///
/// @ingroup pxLineApi
bool setPoint(Line* line, std::size_t index, int x, int y) noexcept;

/// @defgroup pxQuadApi Quad API
///
/// @brief Contains all declarations for quadrilaterals.

/// Sets the stroke color of a quadrilateral.
///
/// @param quad The quadrilateral to modify the color of.
///
/// @ingroup pxQuadApi
void setColor(Quad* quad, float r, float g, float b) noexcept;

/// Sets a point within a quadrilateral.
///
/// @param quad The quadrilateral to set the point of.
/// @param x The X coordinate to assign.
/// @param y The Y coordinate to assign.
///
/// @return True on success, false of @p index is out of bounds.
bool setPoint(Quad* quad, std::size_t index, int x, int y) noexcept;

/// Sets the pixel size of a quadrilateral.
///
/// @param quad The quad to modify the pixel size of.
///
/// @ingroup pxQuadApi
void setPixelSize(Quad* quad, int pixelSize) noexcept;

/// Renders the document onto a color buffer.
///
/// @param doc The document to be rendered.
///
/// @param color The color buffer to render to.
/// There must be 4 floats per color, since the
/// color format is RGBA.
///
/// @param w The width of the color buffer.
/// @param h The height of the color buffer.
void render(const Document* doc, float* color, std::size_t w, std::size_t h) noexcept;

/// Renders the document onto an instance of @ref Image.
///
/// @param doc the document to be rendered.
///
/// @param image The image to render the document onto.
/// This can be generated with @ref createImage
void render(const Document* doc, Image* image) noexcept;

} // namespace px

#endif /* LIBPX_LIBPX_HPP */
