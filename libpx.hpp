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
struct ErrorList;
struct Fill;
struct Image;
struct Layer;
struct Line;
struct Quad;

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

/// Gets a color from a specific pixel on the image.
///
/// @param image The image to get the color from.
/// @param x The X coordinate of the pixel.
/// @param y The Y coordinate of the pixel.
/// @param rgba A pointer to a 4-float storage variable that gets the color.
/// The resultant RGB components are not pre-multiplied.
///
/// @return True on success, false of @p x or @p y were out of bounds.
///
/// @ingroup pxImageApi
bool getColor(const Image* image, std::size_t x, std::size_t y, float* rgba) noexcept;

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

/// @defgroup pxDocumentApi Document API
///
/// @brief Contains all declarations related to the document object.

/// Creates a new document instance.
///
/// @return A new document instance.
///
/// @ingroup pxDocumentApi
Document* createDoc();

/// Imports data from an external document.
///
/// @param doc A pointer to a document returned from @ref createDoc
/// @param filename The path to the file to import the data from.
/// @param errList An optional parameter to store the error list at.
/// See @ref pxErrorListApi for more information.
///
/// @return True on success, false on failure.
bool openDoc(Document* doc, const char* filename, ErrorList** errList = nullptr);

/// Saves a document to a file.
///
/// @param doc The document to save.
/// @param filename The filename to save the data at.
///
/// @return True on success, false on failure.
/// If a failure occurs, no other functions are called
/// that may modify errno. To check the reason for failure,
/// errno would be appropriate.
///
/// @return True on success, false on failure.
///
/// @ingroup pxDocumentApi
bool saveDoc(const Document* doc, const char* filename);

/// Releases memory allocated by a document.
///
/// @param doc The document to release the memory of.
///
/// @ingroup pxDocumentApi
void closeDoc(Document* doc) noexcept;

/// Copies an existing document.
///
/// @exception std::bad_alloc If a memory allocation fails.
///
/// @param other The document to copy.
///
/// @return A pointer to a copy of @p other.
///
/// @ingroup pxDocumentApi
Document* copyDoc(const Document* other);

/// Adds a layer to the document.
///
/// @exception std::bad_alloc If the memory allocation
/// required for this function fails.
///
/// @param doc The document to add the layer to.
///
/// @return A pointer to the layer that was added.
///
/// @ingroup pxDocumentApi
Layer* addLayer(Document* doc);

/// Removes a layer from the document.
///
/// @exception std::out_of_range If the @p index
/// value is greater than or equal to the number of
/// layers in the document.
///
/// @param doc The document to remove the layer from.
/// @param index The index of the layer to remove.
///
/// @ingroup pxDocumentApi
void removeLayer(Document* doc, std::size_t index);

/// Gets the number of layers in a document.
///
/// @param doc The document to get the number of layers of.
///
/// @return The number of layers in the document.
///
/// @ingroup pxDocumentApi
std::size_t getLayerCount(const Document* doc) noexcept;

/// Gets a layer at a specified index.
///
/// @exception std::out_of_range exception if @p index
/// is out of bounds (greater than or equal to the value
/// returned by @ref getLayerCount().
///
/// @param doc The document to get the layer from.
/// @param index The index of the layer to get.
///
/// @return A pointer to the specified layer.
///
/// @ingroup pxDocumentApi
Layer* getLayer(Document* doc, std::size_t index);

/// @copydoc getLayer
const Layer* getLayer(const Document* doc, std::size_t);

/// Moves a layer to a new position.
///
/// @exception std::out_of_range If either of the indices are out of range.
///
/// @param doc The document that the layer is in.
/// @param src The index of the layer to move.
/// @param dst The index to move the layer to.
///
/// @ingroup pxDocumentApi
void moveLayer(Document* doc, std::size_t src, std::size_t dst);

/// Adds a line to a document.
///
/// @param layer The index of the layer to add the line to.
///
/// @return A pointer to a new line instance.
/// The line is owned by the document and does
/// not need to be manually destroyed.
///
/// @ingroup pxDocumentApi
Line* addLine(Document* doc, std::size_t layer = 0);

/// Adds an ellipse to a document.
///
/// @exception std::bad_alloc If the ellipse allocation fails
/// or if the the ellipse array resize fails.
///
/// @param doc The document to add the ellipse to.
/// @param layer The index of the layer to add the line to.
///
/// @return A pointer to the new ellipse instance.
///
/// @ingroup pxDocumentApi
Ellipse* addEllipse(Document* doc, std::size_t layer = 0);

/// Adds a fill operation to the document.
///
/// @param doc The document to add the fill operation to.
/// @param layer The index of the layer to add the line to.
///
/// @return A pointer to a new fill operation.
///
/// @ingroup pxDocumentApi
Fill* addFill(Document* doc, std::size_t layer = 0);

/// Adds a quadrilateral to the document.
///
/// @param doc The document to add the quadrilateral to.
/// @param layer The index of the layer to add the line to.
///
/// @return A pointer to the new quad structure.
///
/// @ingroup pxDocumentApi
Quad* addQuad(Document* doc, std::size_t layer = 0);

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

/// Gets the background color of the document.
///
/// @param doc The document to get the background color of.
/// @param bg A pointer to the float array to store the
/// background color into. This must be able to hold four
/// floats, since the background color is an ARGB vector.
/// The RGB components are not pre multiplied in this case.
void getBackground(const Document* doc, float* bg) noexcept;

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

/// @defgroup pxLayerApi Layer API
///
/// @brief Contains all declarations for layers.

/// Renames the layer.
///
/// @param layer The layer to rename.
/// @param name The name to give the layer.
/// There's no limitations on what characters
/// can go into a layer name and it does not have
/// to be unique within the document.
///
/// @exception std::bad_alloc If the new name
/// can't be allocated.
///
/// @ingroup pxLayerApi
void setLayerName(Layer* layer, const char* name);

/// Gets the name of a layer.
///
/// @param layer The layer to get the name of.
///
/// @return A pointer to the name of the layer.
/// This function never returns a null pointer.
///
/// @ingroup pxLayerApi
const char* getLayerName(const Layer* layer) noexcept;

/// Gets the opacity of a layer.
///
/// @param layer The layer to get the opacity of.
///
/// @return The opacity of @p layer.
///
/// @ingroup pxLayerApi
float getLayerOpacity(const Layer* layer) noexcept;

/// Sets the opacity of the layer.
///
/// @param layer The layer to set the opacity of.
/// @param opacity The opacity to assign the layer.
/// This value should be between 0 and 1.
/// When calling this function, the opacity is clipped into that range.
///
/// @ingroup pxLayerApi
void setLayerOpacity(Layer* layer, float opacity) noexcept;

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

/// @defgroup pxErrorListApi Error List API
///
/// @brief Used for examining errors reporting from opening a file.

/// Releases memory allocated by the error list.
///
/// @param errList The error list to release.
/// This should be the pointer that is assigned
/// from calling @ref openDoc
///
/// @ingroup pxErrorListApi
void closeErrorList(ErrorList* errList) noexcept;

/// Prints the error list to the standard error file.
///
/// @param errList The error list to print.
///
/// @ingroup pxErrorListApi
void printErrorListToStderr(const ErrorList* errList) noexcept;

/// Prints a single error from the error list to the standard error file.
///
/// @param errList The error list to print.
/// @param error The index of the error to print.
///
/// @ingroup pxErrorListApi
void printErrorToStderr(const ErrorList* errList, std::size_t error) noexcept;

/// Gets the total number of errors in the error list.
///
/// @param errList The error list to get the size of.
///
/// @return The total number of errors found.
///
/// @ingroup pxErrorListApi
std::size_t getErrorCount(const ErrorList* errList) noexcept;

/// Gets the position of an error within the source file.
///
/// @param error The index of the error within the error list.
/// See @ref getErrorCount for a range of good values.
///
/// @return The position of the error within the file.
///
/// @ingroup pxErrorListApi
std::size_t getErrorPosition(const ErrorList* errList, std::size_t error) noexcept;

/// Gets the number of characters in the source file that
/// the error pertains to. This can be used with @ref getErrorPosition
/// to determine the range of the error within the source file.
///
/// @return The number of characters that the error pertains to.
///
/// @ingroup pxErrorListApi
std::size_t getErrorSize(const ErrorList* errList, std::size_t error) noexcept;

/// Gets the column that the error begins at within the file.
/// This can be useful to show the user where the error begins at visually.
///
/// @return The column that the error begins at.
/// The first character in a line has a column value of 1.
///
/// @ingroup pxErrorListApi
std::size_t getErrorColumn(const ErrorList* errList, std::size_t error) noexcept;

/// Gets the line number that the error begins at within the file.
/// This can be useful to show the user where the error begins at visually.
///
/// @return The line that the error begins at.
/// The first line in the file has a line number of 1.
///
/// @ingroup pxErrorListApi
std::size_t getErrorLine(const ErrorList* errList, std::size_t error) noexcept;

/// Gets a human-readable description of the error.
///
/// @return A human-readable description of the error.
///
/// @ingroup pxErrorListApi
const char* getErrorDescription(const ErrorList* errList, std::size_t error) noexcept;

/// Gets the source code that the error list pertains to.
/// This can be used to show the source code content that an error is found at.
///
/// @return A pointer to the source code of the document the error list is for.
/// This string is null terminated, but see @getErrorSourceSize to get the size explicitly.
///
/// @ingroup pxErrorListApi
const char* getErrorSource(const ErrorList* errList) noexcept;

/// Gets the number of bytes in the source code that the errors pertain to.
/// While the string returned by @ref getErrorSource is null terminated, there
/// may be null characters in the original file. This ensures that all characters
/// can be seen.
///
/// @return The number of characters in the source code string.
///
/// @ingroup pxErrorListApi
std::size_t getErrorSourceSize(const ErrorList* errList) noexcept;

} // namespace px

#endif /* LIBPX_LIBPX_HPP */
