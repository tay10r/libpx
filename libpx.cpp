#include "libpx.hpp"

#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <vector>

#include <cstring>

namespace px {

namespace {

//======================//
// Section: Scalar Math //
//======================//

/// Calculates the absolute value of a number.
///
/// @tparam T The type of the number to get the absolute value for.
///
/// @param in The value to get the absolute value of.
///
/// @return The absolute value of @p in.
template <typename T>
inline constexpr T absolute(T in) noexcept
{
  return in < 0 ? -in : in;
}

/// Clips a scalar value to be in an inclusive interval
/// of a certain min and maximum.
template <typename T>
inline constexpr T clip(T in, T min = 0, T max = 1) noexcept
{
  return std::max(min, std::min(in, max));
}

//======================//
// Section: Vector Math //
//======================//

/// A generic vector class. This
/// is used for spatial information
/// such as position and direction as
/// well as RGBA values.
///
/// @tparam T The type for each component.
/// This may be an integer for spatial data
/// and a floating point type for color data.
///
/// @tparam dims The number of dimensions in
/// the vector. Generally this is 2 for spatial
/// data and 4 for color data.
template <typename T, std::size_t dims>
struct Vector final
{
  /// One component per dimension.
  /// The first component is X and
  /// the second is Y. This may also
  /// be used for color values, in which
  /// case the first component is red, the
  /// second is green, the third is blue, and
  /// the last is alpha.
  T data[dims] {};
  /// Accesses a component in the vector.
  constexpr inline T& operator [] (std::size_t index) noexcept {
    return data[index];
  }
  /// Accesses a component in the vector.
  constexpr inline const T& operator [] (std::size_t index) const noexcept {
    return data[index];
  }
  /// Adds two vectors.
  inline constexpr Vector operator + (const Vector& other) const noexcept
  {
    Vector out;

    for (std::size_t i = 0; i < dims; i++) {
      out.data[i] = data[i] + other.data[i];
    }

    return out;
  }
  /// Subtracts two vectors.
  inline constexpr Vector operator - (const Vector& other) const noexcept
  {
    Vector out;

    for (std::size_t i = 0; i < dims; i++) {
      out.data[i] = data[i] - other.data[i];
    }

    return out;
  }
  /// Adds a scalar value to the vector.
  inline constexpr Vector operator + (T n) const noexcept
  {
    Vector out;

    for (std::size_t i = 0; i < dims; i++) {
      out.data[i] = data[i] + n;
    }

    return out;
  }
  /// Subtracts a scalar value from the vector.
  inline constexpr Vector operator - (T n) const noexcept
  {
    Vector out;

    for (std::size_t i = 0; i < dims; i++) {
      out.data[i] = data[i] - n;
    }

    return out;
  }
  /// Divides the vector by a scalar value.
  inline constexpr Vector operator / (T n) const noexcept
  {
    Vector out;

    for (std::size_t i = 0; i < dims; i++) {
      out.data[i] = data[i] / n;
    }

    return out;
  }
  /// Multiplies the vector by a scalar value.
  inline constexpr Vector operator * (T n) const noexcept
  {
    Vector out;

    for (std::size_t i = 0; i < dims; i++) {
      out.data[i] = data[i] * n;
    }

    return out;
  }
};

/// Calculates a vector with all absolute value components.
///
/// @param v The vector to get the absolute values of.
///
/// @return The resultant vector.
template <typename T, std::size_t dims>
inline constexpr Vector<T, dims> absolute(const Vector<T, dims>& v) noexcept
{
  Vector<T, dims> out;

  for (std::size_t i = 0; i < dims; i++) {
    out.data[i] = absolute(v[i]);
  }

  return out;
}

/// Calculates a vector whose components
/// are the minimum values between two other vectors.
template <typename T, std::size_t dims>
inline constexpr Vector<T, dims> min(const Vector<T, dims>& a, const Vector<T, dims>& b) noexcept
{
  Vector<T, dims> out;

  for (std::size_t i = 0; i < dims; i++) {
    out.data[i] = std::min(a.data[i], b.data[i]);
  }

  return out;
}

/// Calculates a vector whose components
/// are the maximum values between two other vectors.
template <typename T, std::size_t dims>
inline constexpr Vector<T, dims> max(const Vector<T, dims>& a, const Vector<T, dims>& b) noexcept
{
  Vector<T, dims> out;

  for (std::size_t i = 0; i < dims; i++) {
    out.data[i] = std::max(a.data[i], b.data[i]);
  }

  return out;
}

/// A color type definition.
using Color = Vector<float, 4>;

/// A 2D vector type definition.
/// Since this is usually used for
/// spatial information, it is an
/// integer vector.
using Vec2 = Vector<int, 2>;

/// This is the change in value of a
/// color channel that is seen from the
/// final result of an 8-bit per channel image.
/// This value is primarily used to see if
/// two colors are mostly equal.
constexpr float colorDelta() noexcept { return 1.0f / 256.0f; }

/// Indicates if two colors are almost equal.
///
/// @param a The first color operand.
/// @param b The second color operand.
/// @param bias The maximum allowable difference.
/// This defaults to (1 / 256), which is the
/// user perceivable difference.
///
/// @return True if they're almost equal, false otherwise.
constexpr bool almostEqual(const Color& a, const Color& b, float bias = colorDelta()) noexcept
{
  auto diff = absolute(a - b);

  return (diff[0] < bias)
      && (diff[1] < bias)
      && (diff[2] < bias)
      && (diff[3] < bias);
}

/// Clips a color to be between a minimum and maximum value.
///
/// @param in The color to clip.
/// @param min The minimum value to clip to.
/// @param max The maximum value to clip to.
constexpr Color clip(const Color& in, float min = 0, float max = 1) noexcept
{
  return Color {
    std::min(std::max(min, in[0]), max),
    std::min(std::max(min, in[1]), max),
    std::min(std::max(min, in[2]), max),
    std::min(std::max(min, in[3]), max)
  };
}

/// Indicates if two 2D vectors are equal.
inline bool operator == (const Vec2& a, const Vec2& b) noexcept
{
  return (a[0] == b[0]) && (a[1] == b[1]);
}

/// Clips a vector between a minimum and a maximum range.
///
/// @param in The vector to clip.
/// @param min The minimum range of the vector.
/// @param max The maximum range of the vector.
///
/// @return The resultant vector.
inline Vec2 clip(const Vec2& in, const Vec2& min, const Vec2& max) noexcept
{
  return Vec2 {
    std::min(std::max(in[0], min[0]), max[0]),
    std::min(std::max(in[1], min[1]), max[1])
  };
}

} // namespace

//===================//
// Section: Geometry //
//===================//

namespace {

//constexpr Color white() noexcept { return Color { 1, 1, 1, 1 }; }
constexpr Color black() noexcept { return Color { 0, 0, 0, 1 }; }
constexpr Color transparent() noexcept { return Color { 0, 0, 0, 0 }; }

/// Used for visiting nodes in the scene graph.
class NodeAccessor
{
public:
  /// Just a stub.
  virtual ~NodeAccessor() {}
  virtual void access(const Ellipse& ellipse) noexcept = 0;
  virtual void access(const Fill& fill) noexcept = 0;
  virtual void access(const Line& line) noexcept = 0;
  virtual void access(const Quad& quad) noexcept = 0;
};

/// This is the base of any
/// class that appears in the scene graph.
struct Node
{
  /// Just a stub.
  virtual ~Node() {}
  /// Allows a node accessor class access
  /// to the derived node type.
  virtual void accept(NodeAccessor& accessor) const noexcept = 0;
  /// Copies the derived node.
  virtual Node* copy() const = 0;
};

/// A type definition for a node smart pointer.
using NodePtr = std::unique_ptr<Node>;

/// This is the base of any class that has
/// a stroke. It contains the basic properties
/// of how the stroke should be drawn.
struct StrokeNode : public Node
{
  /// The size of the squares that
  /// are drawn along the stroke.
  std::size_t pixelSize = 1;
  /// The color that the stroke is drawn with.
  Color color = black();
};

/// Evaluates a number to a safe pixel size.
/// A pixel size must be greater than or equal to 1.
inline constexpr int safePixelSize(int in) noexcept
{
  return (in <= 0) ? 1 : in;
}

} // namespace

struct Ellipse final : public StrokeNode
{
  Vec2 center = Vec2 { 0, 0 };
  Vec2 radius = Vec2 { 0, 0 };

  void accept(NodeAccessor& accessor) const noexcept override
  {
    accessor.access(*this);
  }

  Node* copy() const override
  {
    return new Ellipse(*this);
  }
};

void setCenter(Ellipse* ellipse, int x, int y) noexcept
{
  ellipse->center = Vec2 { x, y };
}

void setRadius(Ellipse* ellipse, int x, int y) noexcept
{
  ellipse->radius = Vec2 { x, y };
}

void setColor(Ellipse* ellipse, float r, float g, float b) noexcept
{
  ellipse->color = clip(Color { r, g, b, 1 });
}

void setPixelSize(Ellipse* ellipse, int pixelSize) noexcept
{
  ellipse->pixelSize = safePixelSize(pixelSize);
}

void resizeRect(Ellipse* ellipse, int x1, int y1, int x2, int y2) noexcept
{
  auto p1 = Vec2 { x1, y1 };
  auto p2 = Vec2 { x2, y2 };

  auto pMin = min(p1, p2);
  auto pMax = max(p1, p2);

  ellipse->center = (pMax + pMin) / 2;
  ellipse->radius = (pMax - pMin) / 2;
}

/// Represents a flood fill operation.
struct Fill final : public Node
{
  /// The color to fill the area with.
  Color color = black();
  /// The position on the image to start the fill operation at.
  /// All pixels connected to this point are filled.
  Vec2 origin = Vec2 { 0, 0 };

  void accept(NodeAccessor& accessor) const noexcept override
  {
    accessor.access(*this);
  }

  Node* copy() const override
  {
    return new Fill(*this);
  }
};

void setFillOrigin(Fill* fill, int x, int y) noexcept
{
  fill->origin = Vec2 { x, y };
}

void setColor(Fill* fill, float r, float g, float b) noexcept
{
  fill->color = clip(Color { r, g, b, 1 });
}

/// Represents a series of straight line segments.
struct Line final : public StrokeNode
{
  /// The points making up the line.
  std::vector<Vec2> points;

  void accept(NodeAccessor& accessor) const noexcept override
  {
    accessor.access(*this);
  }

  Node* copy() const override
  {
    return new Line(*this);
  }
};

void addPoint(Line* line, int x, int y)
{
  // Check for duplicate point.
  if (line->points.size() > 0) {
    const auto& last = line->points[line->points.size() - 1];
    if ((last[0] == x) && (last[1] == y)) {
      return;
    }
  }

  line->points.emplace_back(Vec2 { x, y });
}

bool setPoint(Line* line, std::size_t index, int x, int y) noexcept
{
  if (index >= line->points.size()) {
    return false;
  } else {
    line->points[index] = Vec2 { x, y };
    return true;
  }
}

void setPixelSize(Line* line, int pixelSize) noexcept
{
  line->pixelSize = safePixelSize(pixelSize);
}

void setColor(Line* line, float r, float g, float b) noexcept
{
  line->color = clip(Color { r, g, b, 1 });
}

/// Represents a quadrilateral shape.
/// A quadrilateral shape differs from
/// a rectangle in that the lines do not
/// have to be axis-aligned.
struct Quad final : public StrokeNode
{
  /// The points making up the quadrilateral.
  Vec2 points[4] { Vec2 { 0, 0 }, Vec2 { 1, 0 }, Vec2 { 1, 1 }, Vec2 { 0, 1 } };

  void accept(NodeAccessor& accessor) const noexcept override
  {
    accessor.access(*this);
  }
  Node* copy() const override
  {
    return new Quad(*this);
  }
};

bool setPoint(Quad* quad, std::size_t index, int x, int y) noexcept
{
  if (index >= 4) {
    return false;
  } else {
    quad->points[index] = Vec2 { x, y };
    return true;
  }
}

void setColor(Quad* quad, float r, float g, float b) noexcept
{
  quad->color = clip(Color { r, g, b, 1 });
}

void setPixelSize(Quad* quad, int pixelSize) noexcept
{
  quad->pixelSize = safePixelSize(pixelSize);
}

//=================//
// Section: Layers //
//=================//

/// A layer here is what it is in most image
/// editing applications, a collection of 2D data
/// that is meant for a certain Z index and opacity,
/// to be drawn in a certain order relative to the other layers.
struct Layer final
{
  /// The alpha channel value of this layer.
  float opacity = 1;
  /// The name given to this layer.
  std::string name;
  /// The nodes for this layer
  std::vector<NodePtr> nodes;
  /// Just a stub.
  Layer() {}
  /// Copies a layer.
  Layer(const Layer& other)
  {
    opacity = other.opacity;
    name = other.name;
    for (const auto& otherNode : other.nodes) {
      nodes.emplace_back(otherNode->copy());
    }
  }
  /// Adds a node to the layer.
  ///
  /// @tparam NodeType The type of the node to add.
  ///
  /// @param node The node to add to the layer.
  ///
  /// @return A pointer to to @p node.
  template <typename NodeType>
  NodeType* addNode(NodeType* node)
  {
    // In case an exception gets thrown.
    std::unique_ptr<NodeType> nodePtr(node);

    nodes.emplace_back(node);

    return nodePtr.release();
  }
};

/// A type definition for a layer smart pointer.
using LayerPtr = std::unique_ptr<Layer>;

const char* getLayerName(const Layer* layer) noexcept
{
  return layer->name.c_str();
}

float getLayerOpacity(const Layer* layer) noexcept
{
  return layer->opacity;
}

void setLayerName(Layer* layer, const char* name)
{
  layer->name = name ? name : "";
}

void setLayerOpacity(Layer* layer, float opacity) noexcept
{
  layer->opacity = opacity;
}

//================//
// Section: Image //
//================//

/// Contains basic image data.
struct Image final
{
  /// The image colors, formatted
  /// in the order of RGBA.
  std::vector<float> colorBuffer;
  /// The width of the image, in pixels.
  std::size_t width = 0;
  /// The height of the image, in pixels.
  std::size_t height = 0;
};

Image* createImage(std::size_t width, std::size_t height)
{
  auto image = std::make_unique<Image>(Image());

  resizeImage(image.get(), width, height);

  return image.release();
}

void closeImage(Image* image) noexcept
{
  delete image;
}

const float* getColorBuffer(const Image* image) noexcept
{
  return image->colorBuffer.data();
}

bool getColor(const Image* image, std::size_t x, std::size_t y, float* rgba) noexcept
{
  if ((x >= image->width)
   || (y >= image->height)) {
    return false;
  }

  const auto* src = &image->colorBuffer[((y * image->width) + x) * 4];

  // TODO : Restore RGB before multiply
  rgba[0] = src[0];
  rgba[1] = src[1];
  rgba[2] = src[2];
  rgba[3] = src[3];

  return true;
}

std::size_t getImageWidth(const Image* image) noexcept { return image->width; }

std::size_t getImageHeight(const Image* image) noexcept { return image->height; }

void resizeImage(Image* image, std::size_t w, std::size_t h)
{
  image->colorBuffer.resize(w * h * 4);
  image->width = w;
  image->height = h;
}

//=====================//
// Section: Error List //
//=====================//

/// Contains information on a single error.
struct Error final
{
  /// The stream used to format the error.
  /// This isn't seen by the end user but it can
  /// be used to make error reporting easier.
  std::ostringstream stream;
  /// A human-readable description of the error.
  std::string description;
  /// The line that the error can be found at.
  std::size_t line = 0;
  /// The column that the error can be found at.
  std::size_t column = 0;
  /// The index within the file that the error begins at.
  std::size_t index = 0;
  /// The number of characters that the error pertains to.
  std::size_t size = 0;
};

/// Prints an error to a stream.
///
/// @param stream The stream to print to.
/// @param err The error to print.
///
/// @return A reference to @p stream.
std::ostream& operator << (std::ostream& stream, const Error& err)
{
  return stream << err.line << ':' << err.column << ": " << err.description.c_str();
}

struct ErrorList final
{
  /// The path to the file that was opened.
  std::string filename;
  /// The source code that the errors pertain to.
  /// This is all the source code in the original file.
  std::string source;
  /// The list of errors that were found.
  std::vector<Error> errors;
};

void closeErrorList(ErrorList* errList) noexcept
{
  delete errList;
}

void printErrorListToStderr(const ErrorList* errList) noexcept
{
  for (std::size_t i = 0; i < errList->errors.size(); i++) {
    printErrorToStderr(errList, i);
  }
}

void printErrorToStderr(const ErrorList* errList, std::size_t index) noexcept
{
  if (index >= errList->errors.size()) {
    return;
  }

  std::cerr << errList->filename << ':' << errList->errors[index] << std::endl;
}

const char* getErrorSource(const ErrorList* errList) noexcept
{
  return errList->source.c_str();
}

std::size_t getErrorSourceSize(const ErrorList* errList) noexcept
{
  return errList->source.size();
}

std::size_t getErrorCount(const ErrorList* errList) noexcept
{
  return errList->errors.size();
}

std::size_t getErrorColumn(const ErrorList* errList, std::size_t error) noexcept
{
  if (error >= errList->errors.size()) {
    return 0;
  }

  return errList->errors[error].column;
}

std::size_t getErrorLine(const ErrorList* errList, std::size_t error) noexcept
{
  if (error >= errList->errors.size()) {
    return 0;
  }

  return errList->errors[error].line;
}

std::size_t getErrorPosition(const ErrorList* errList, std::size_t error) noexcept
{
  if (error >= errList->errors.size()) {
    return 0;
  }

  return errList->errors[error].index;
}

std::size_t getErrorSize(const ErrorList* errList, std::size_t error) noexcept
{
  if (error >= errList->errors.size()) {
    return 0;
  }

  return errList->errors[error].size;
}

const char* getErrorDescription(const ErrorList* errList, std::size_t error) noexcept
{
  if (error >= errList->errors.size()) {
    return 0;
  }

  return errList->errors[error].description.c_str();
}

//========================//
// Section: Serialization //
//========================//

namespace {

/// This value indicates the resolution
/// at which floating point colors are
/// converting to integer values for encoding
/// and decoding.
constexpr std::size_t colorRes() noexcept
{
  return 32768;
}

/// Prints an nth dimensional vector.
///
/// @tparam T The type used in the vector components.
/// @tparam dims The number of dimensions in the vector.
///
/// @param v The vector to print.
template <typename T, std::size_t dims>
std::ostream& operator << (std::ostream& stream, const Vector<T, dims>& v)
{
  for (std::size_t i = 0; i < dims; i++) {

    stream << v[i];

    if ((i + 1) < dims) {
      stream << ' ';
    }
  }

  return stream;
}

/// Prints an array of vectors.
/// The appear as a single list of numbers this way.
///
/// @tparam T The type used in the vector components.
/// @tparam dims The number of dimensions in the vector.
///
/// @param v The vector array to print.
template <typename T, std::size_t dims>
std::ostream& operator << (std::ostream& stream, const std::vector<Vector<T, dims>>& v)
{
  for (std::size_t i = 0; i < v.size(); i++) {

    stream << v[i];

    if ((i + 1) < v.size()) {
      stream << ' ';
    }
  }

  return stream;
}

/// Used for encoding documents into files.
class Encoder final : public NodeAccessor
{
  /// The stream being written to.
  std::ostream& stream;
  /// The indentation level for the output file.
  std::size_t indentation = 0;
public:
  Encoder(std::ostream& stream_) : stream(stream_) {}
  /// Encodes a single color channel.
  ///
  /// @param name The name to give the channel.
  /// @param value The value to encode.
  void encodeColorChannel(const char* name, float value)
  {
    encodeSize(name, std::size_t(clip(value) * colorRes()));
  }
  /// Encodes a color.
  ///
  /// @param name The name to give the color.
  /// @param c The color to encode.
  void encodeColor(const char* name, const Color& c)
  {
    indent() << name << ' ' << convertColor(c) << std::endl;
  }
  /// Encodes a size field.
  ///
  /// @param name The name to give the field.
  /// @param value The value to print.
  void encodeSize(const char* name, std::size_t value)
  {
    indent() << name << ' ' << value << std::endl;
  }
  /// Encodes a string onto the document.
  ///
  /// @param name The name to give the string field.
  /// @param value The string value to add.
  void encodeString(const char* name, const char* value)
  {
    indent() << name << " \"";

    for (std::size_t i = 0; value[i] != 0; i++) {

      if ((value[i] == '\"') || (value[i] == '\\')) {
        stream << '\\';
      }

      stream << value[i];
    }

    stream << "\"" << std::endl;
  }
  /// Encodes a layer.
  void encodeLayer(const Layer& layer)
  {
    auto encoder = [this, &layer]() {
      encodeString("name", layer.name.c_str());
      encodeColorChannel("opacity", layer.opacity);
      for (const auto& node : layer.nodes) {
        node->accept(*this);
      }
    };

    encodeStruct("layer", encoder);
  }
protected:
  /// Converts a color into a 4 dimensional integer vector.
  ///
  /// This is required to keep the parsing simple.
  static Vector<int, 4> convertColor(const Color& c) noexcept
  {
    return Vector<int, 4> {
      int(c[0] * colorRes()),
      int(c[1] * colorRes()),
      int(c[2] * colorRes()),
      int(c[3] * colorRes())
    };
  }
  /// Prints indentation.
  std::ostream& indent() {
    for (std::size_t i = 0; i < indentation; i++) {
      stream << "  ";
    }
    return stream;
  }
  /// Encodes a structure.
  /// The inner structure is printed with a lambda.
  /// The indentation is increased before the lambda
  /// is called and restored after the lambda.
  template <typename Functor>
  void encodeStruct(const char* name, Functor func)
  {
    indent() << name << std::endl;

    indentation++;

    func();

    indentation--;

    indent() << "end" << std::endl;
  }
  /// Encodes a stroke node.
  /// This is used by all derived of this class,
  /// so it must be called explicitly.
  void encodeStrokeNode(const StrokeNode& strokeNode)
  {
    indent() << "pixel_size " << strokeNode.pixelSize << std::endl;
    indent() << "color " << convertColor(strokeNode.color) << std::endl;
  }
  void access(const Ellipse& ellipse) noexcept override
  {
    auto encoder = [this, ellipse] () {
      encodeStrokeNode(ellipse);
      indent() << "center " << ellipse.center << std::endl;
      indent() << "radius " << ellipse.radius << std::endl;
    };

    encodeStruct("ellipse", encoder);
  }
  void access(const Fill& fill) noexcept override
  {
    auto encoder = [this, fill] () {
      indent() << "origin " << fill.origin << std::endl;
      indent() << "color " << convertColor(fill.color) << std::endl;
    };

    encodeStruct("fill", encoder);
  }
  void access(const Line& line) noexcept override
  {
    auto encoder = [this, line] () {
      encodeStrokeNode(line);
      indent() << "points " << line.points << " end" << std::endl;
    };

    encodeStruct("line", encoder);
  }
  void access(const Quad& quad) noexcept override
  {
    auto encoder = [this, quad] () {
      encodeStrokeNode(quad);
      indent();
      stream << "points ";
      stream << quad.points[0] << ' ';
      stream << quad.points[1] << ' ';
      stream << quad.points[2] << ' ';
      stream << quad.points[3] << std::endl;
    };

    encodeStruct("quad", encoder);
  }
};

} // namespace

//==========================//
// Section: Deserialization //
//==========================//

namespace {

/// Enumerates the several recognized tokens.
enum class TokenType
{
  /// A placeholder until initialized.
  None,
  /// A single line comment beginning with '#'
  Comment,
  /// A C-style identifier (a-zA-Z_)
  Identifier,
  /// An integer (with optional leading '-')
  Integer,
  /// A whitespace character (space, tab, newline sequences)
  Space,
  /// A double quoted string value.
  StringLiteral,
  /// An invalid character
  Invalid
};

struct Token final
{
  /// The data from the file.
  const char* data = "";
  /// The number of characters in the token.
  std::size_t size = 0;
  /// The position of the token in the original file.
  std::size_t pos = 0;
  /// The line that the token begins at.
  std::size_t line = 0;
  /// The column that the token begins at.
  std::size_t column = 0;
  /// The type of this token.
  TokenType type = TokenType::None;
  /// Indicates if the token is valid or not.
  operator bool () const noexcept {
    return type != TokenType::None;
  }
  /// Checks for equality with a certain string literal.
  bool operator == (const char* id) noexcept
  {
    auto idSize = std::strlen(id);
    if (idSize != size) {
      return false;
    }

    return std::memcmp(id, data, size) == 0;
  }
  /// Checks for non-equality with a certain string literal.
  bool operator != (const char* id) noexcept
  {
    return !(*this == id);
  }
  /// Checks for equality with another token type.
  inline constexpr bool operator == (TokenType t) noexcept
  {
    return type == t;
  }
  /// Checks for non-equality with another token type.
  inline constexpr bool operator != (TokenType t) noexcept
  {
    return type != t;
  }
};

/// Prints the contents of a token into a stream.
std::ostream& operator << (std::ostream& output, const Token& t)
{
  output << '"';
  for (std::size_t i = 0; i < t.size; i++) {
    output << t.data[i];
  }
  output << '"';
  return output;
}

/// Scans a string for parse-able tokens.
class Lexer final
{
  /// The string being parsed.
  const char* data = "";
  /// The size of the string.
  std::size_t size = 0;
  /// The current position of the lexer.
  std::size_t pos = 0;
  /// The current line the lexer is at.
  std::size_t line = 1;
  /// The current column the lexer is at.
  std::size_t column = 1;
public:
  constexpr Lexer(const char* d, std::size_t s)
    : data(d), size(s) {}
  /// Scans the input for a token.
  Token scan() noexcept
  {
    auto t = space();
    if (t) {
      return t;
    }

    t = identifier();
    if (t) {
      return t;
    }

    t = number();
    if (t) {
      return t;
    }

    t = stringLiteral();
    if (t) {
      return t;
    }

    t = comment();
    if (t) {
      return t;
    }

    return fallbackToken();
  }
  /// Gets the current lexer position.
  /// This value can be used to backtrack if necessary.
  std::size_t getPosition() const noexcept { return pos; }
  /// Indicates the total number of characters remaining.
  inline constexpr std::size_t remaining() const noexcept
  {
    return (pos < size) ? (size - pos) : 0;
  }
  /// Assigns the position of the next scan operation.
  void setPosition(std::size_t p) noexcept { pos = p; }
protected:
  /// Creates a token as a last resort for the caller.
  /// If there is input, one character is used and is
  /// considered to be "invalid." If there is no input,
  /// then an empty token is returned.
  Token fallbackToken() noexcept
  {
    if (remaining() > 0) {
      return makeToken(TokenType::Invalid, 1);
    } else {
      return Token();
    }
  }
  /// Scans for a comment token.
  Token comment() noexcept
  {
    std::size_t match = 0;

    if (!isEqual(match, '#')) {
      return Token();
    } else {
      match++;
    }

    while (inBounds(match) && !isEqual(match, '\n') && !isEqual(match, '\r')) {
      match++;
    }

    return makeToken(TokenType::Comment, match);
  }
  /// Scans for an identifier token.
  Token identifier() noexcept
  {
    std::size_t match = 0;

    if (!isInRange(match, 'a', 'z')
     && !isInRange(match, 'A', 'Z')
     && !isEqual(match, '_')) {
      return Token();
    } else {
      match++;
    }

    while (inBounds(match)) {
      if (isInRange(match, 'a', 'z')
       || isInRange(match, 'A', 'Z')
       || isInRange(match, '0', '9')
       || isEqual(match, '_')) {
        match++;
      } else {
        break;
      }
    }

    if (match > 0) {
      return makeToken(TokenType::Identifier, match);
    } else {
      return Token();
    }
  }
  /// Scans for a number token.
  Token number() noexcept
  {
    auto neg = false;

    std::size_t match = 0;

    if (isEqual(match, '-')) {
      neg = true;
      match++;
    }

    while (inBounds(match)) {
      if (isInRange(match, '0', '9')) {
        match++;
      } else {
        break;
      }
    }

    if ((neg && (match > 1)) || (!neg && (match > 0))) {
      return makeToken(TokenType::Integer, match);
    } else {
      return Token();
    }
  }
  /// Scans for a space token.
  Token space() noexcept
  {
    std::size_t match = 0;

    while (inBounds(match)) {
      if (isEqual(match, ' ')
       || isEqual(match, '\t')
       || isEqual(match, '\n')
       || isEqual(match, '\r')) {
        match++;
      } else {
        break;
      }
    }

    if (match > 0) {
      return makeToken(TokenType::Space, match);
    } else {
      return Token();
    }
  }
  /// Scans for a double quoted string literal.
  Token stringLiteral() noexcept
  {
    std::size_t match = 0;

    if (!isEqual(match, '"')) {
      return Token();
    } else {
      match++;
    }

    while (inBounds(match)) {

      if (isEqual(match, '\\')) {
        match++;
      } else if (isEqual(match, '"')) {
        break;
      }

      match++;
    }

    if (!isEqual(match, '"')) {
      // We could either mark '"' as invalid by itself
      // or everything up to this point. Marking everything
      // up to this point reduces the amount of false errors
      // to zero, so that's what we'll do.
      return makeToken(TokenType::Invalid, match);
    }

    return makeToken(TokenType::StringLiteral, match + 1);
  }
  /// Creates a token of a certain type and size.
  inline constexpr Token makeToken(TokenType type, std::size_t s) noexcept
  {
    Token token {
      data + pos,
      s,
      pos,
      line,
      column,
      type
    };

    next(s);

    return token;
  }
  /// Indicates if a character is equal to another.
  inline constexpr bool isEqual(std::size_t offset, char c) const noexcept
  {
    return look(offset) == c;
  }
  /// Indicates if a character is in a certain range.
  inline constexpr bool isInRange(std::size_t offset, char begin, char end) const noexcept
  {
    char c = look(offset);
    return (c >= begin) && (c <= end);
  }
  /// Indicates if an offset is out of bounds.
  inline constexpr bool inBounds(std::size_t offset) const noexcept
  {
    return (pos + offset) < size;
  }
  /// Goes passed a certain number of characters.
  inline constexpr void next(std::size_t count) noexcept
  {
    for (std::size_t i = 0; (i < count) && (pos < size); i++) {

      auto c = data[pos];
      if (c == '\n') {
        line++;
        column = 1;
      } else if (c > 0) {
        column++;
      }

      pos++;
    }
  }
  /// Used for checking the contents of a character
  /// a certain offset array from the current position.
  inline constexpr char look(std::size_t offset = 0) const noexcept
  {
    return ((pos + offset) < size) ? data[pos + offset] : 0;
  }
};

/// Used for storing optional results.
/// Meant primarily for POD.
template <typename T>
struct Optional final
{
  T value = T();
  bool valid = false;

  constexpr Optional() {}
  constexpr Optional(const T& v) : value(v), valid(true) {}
};

/// A recursive-decent parser for parsing
/// document files. The parser fails immediately
/// after finding the first error.
class Parser final
{
  /// Suppressed error messages go here.
  std::ostringstream suppressed;
  /// The tokens found by the lexer.
  std::vector<Token> tokens;
  /// The position of the parser among the tokens.
  std::size_t pos = 0;
  /// Whether or not the parser has failed.
  bool failedFlag = false;
  /// The list of errors found by the parser.
  ErrorList errorList;
public:
  /// Constructs a new parser instance.
  /// This function will parse the string and
  /// filter out any tokens that the parser
  /// will not need.
  ///
  /// @param str The string to parse.
  /// @param size The number of characters in @p str.
  Parser(const char* str, std::size_t size)
  {
    Lexer lexer(str, size);

    while (lexer.remaining() && !failed()) {
      auto t = lexer.scan();
      if (t == TokenType::Invalid) {
        formatError(t) << "Invalid token " << t;
        break;
      } else if (t == TokenType::None) {
        break;
      } else if ((t == TokenType::Space) || (t == TokenType::Comment)) {
        continue;
      } else {
        tokens.emplace_back(t);
      }
    }
  }
  /// Indicates whether or not the parser failed.
  inline constexpr bool failed() const noexcept { return failedFlag; }
  /// Gets the error list found by the parser.
  /// Future calls to this function will return
  /// an empty error list.
  ///
  /// @param content The variable containing the origin source code.
  /// This is assigned to the error list so that the context of the
  /// error can be shown if needed.
  ErrorList* getErrorList(const char* filename, std::string&& content)
  {
    // Resolve the stream contents to the descriptions.
    for (auto& err : errorList.errors) {
      err.description = err.stream.str();
    }

    errorList.filename = filename;
    errorList.source = std::move(content);

    return new ErrorList(std::move(errorList));
  }
  /// This can be called when nothing is available
  /// to parse and there is still remaining tokens
  /// in the parser. It emits an error message describing
  /// the bad token and moves the parser past it.
  void badToken()
  {
    auto tok = look();
    formatError(tok) << "Invalid token " << tok;
    next();
  }
  /// Attempts to parse a layer node.
  ///
  /// @return A pointer to a layer node, if one is found.
  /// Otherwise, a null pointer is returned.
  LayerPtr parseLayer()
  {
    auto firstTok = look();

    if (!matchID("layer")) {
      return LayerPtr();
    }

    LayerPtr layer(new Layer());

    while (remaining() && !failed() && !matchID("end")) {

      auto str = parseString("name");
      if (str.valid) {
        layer->name = str.value;
        continue;
      }

      auto opacity = parseColorChannel("opacity");
      if (opacity.valid) {
        layer->opacity = opacity.value;
        continue;
      }

      auto node = parseNode();
      if (node) {
        layer->nodes.emplace_back(std::move(node));
        continue;
      }

      if (failed()) {
        return LayerPtr();
      } else {
        formatError(firstTok) << "Missing 'end' statement.";
        return LayerPtr();
      }
    }

    return layer;
  }
  /// Parses for a node.
  ///
  /// @return On success, a pointer to a node.
  /// On failure, a null pointer.
  NodePtr parseNode()
  {
    auto node = parseLineNode();
    if (node) {
      return node;
    }

    node = parseEllipseNode();
    if (node) {
      return node;
    }

    node = parseQuadNode();
    if (node) {
      return node;
    }

    node = parseFillNode();
    if (node) {
      return node;
    }

    return NodePtr();
  }
  /// Parses for a color value.
  ///
  /// @param name The name of the color value to parse for.
  ///
  /// @return Optionally returns a color if the correct one was found.
  Optional<Color> parseColor(const char* name) noexcept
  {
    auto nameTok = look();

    if (!matchID(name)) {
      return Optional<Color>();
    }

    auto v = parseVector<4>();
    if (!v.valid) {
      formatError(nameTok) << "Failed to match color values following " << nameTok;
      return Optional<Color>();
    }

    return Optional<Color>(toColor(v.value));
  }
  /// Attempts to parse a string.
  ///
  /// @param name The name of the string to parse.
  ///
  /// @return Optionally returns a string.
  Optional<std::string> parseString(const char* name) noexcept
  {
    if (!matchID(name)) {
      return Optional<std::string>();
    }

    auto str = parseString();
    if (!str.valid) {
      return Optional<std::string>();
    }

    return str;
  }
  /// Attempts to parse a color channel.
  ///
  /// @param name The name of the color channel value.
  ///
  /// @return Optionally returns a color channel value.
  Optional<int> parseColorChannel(const char* name) noexcept
  {
    auto nameTok = look();

    if (!matchID(name)) {
      return Optional<int>();
    }

    auto i = parseInt();
    if (!i.valid) {
      formatError(nameTok) << "Failed to match color channel value following " << nameTok;
      return Optional<int>();
    }

    return i;
  }
  /// Parses for a string literal.
  Optional<std::string> parseString()
  {
    auto tok = look();
    if (tok != TokenType::StringLiteral) {
      formatError(tok) << "Expected a string literal, but got " << tok;
      return Optional<std::string>();
    }

    std::string result;

    for (std::size_t i = 1; (i + 1) < tok.size; i++) {

      auto c = tok.data[i];

      if (c == '\\') {
        if ((i + 1) >= tok.size) {
          parserError(tok, __LINE__);
          return Optional<std::string>();
        }
        auto next = tok.data[i + 1];
        if (next == 'r') {
          result += '\r';
        } else if (next == 'n') {
          result += '\n';
        } else if (next == '\\') {
          result += '\\';
        } else if (next == '"') {
          result += '"';
        } else {
          formatError(tok) << "Invalid escape character found.";
          return Optional<std::string>();
        }
        i++;
      } else {
        result += c;
      }
    }

    next();

    return Optional<std::string>(std::move(result));
  }
  /// Parses for an integer vector.
  ///
  /// @tparam dims The number of dimensions used in the vector.
  ///
  /// @return Optionally returns the integer.
  template <std::size_t dims>
  Optional<Vector<int, dims>> parseVector() noexcept
  {
    using Result = Optional<Vector<int, dims>>;

    auto memo = pos;

    Vector<int, dims> out;

    for (std::size_t i = 0; i < dims; i++) {
      auto component = parseInt();
      if (!component.valid) {
        pos = memo;
        return Result();
      } else {
        out[i] = component.value;
      }
    }

    return Result(out);
  }
  /// Parses a named vector.
  ///
  /// @param name The name of the vector to parse.
  ///
  /// @return Optionally returns the vector if it is matched.
  template <std::size_t dims>
  Optional<Vector<int, dims>> parseVector(const char* name) noexcept
  {
    using Result = Optional<Vector<int, dims>>;

    auto nameTok = look();

    if (!matchID(name)) {
      return Result();
    }

    auto v = parseVector<dims>();
    if (!v.valid) {
      formatError(nameTok) << "Failed to match vector following " << nameTok;
      return Result();
    }

    return v;
  }
  /// Parses for a single integer value.
  ///
  /// @parame name The name of the value.
  Optional<int> parseInt(const char* name) noexcept
  {
    auto firstTok = look();

    if (!matchID(name)) {
      return Optional<int>();
    }

    auto i = parseInt();
    if (!i.valid) {
      formatError(firstTok) << "Failed to parse integer following " << firstTok;
      return Optional<int>();
    }

    return i;
  }
  /// Parses for an integer value from
  /// the next token.
  Optional<int> parseInt() noexcept
  {
    auto numberToken = look();
    if (numberToken != TokenType::Integer) {
      return Optional<int>();
    }

    next();

    return parseInt(numberToken);
  }
  /// Parses a size value.
  ///
  /// @param name The name of the size value to parse.
  ///
  /// @return Optionally returns the size.
  Optional<std::size_t> parseSize(const char* name) noexcept
  {
    auto tmp = parseInt(name);
    if (!tmp.valid) {
      return Optional<std::size_t>();
    } else if (tmp.value < 0) {
      formatError(previousTok()) << "Expected '" << name << "' to be positive.";
      return Optional<std::size_t>();
    }

    return Optional<std::size_t>(tmp.value);
  }
  /// Indicates the number of tokens remaining to be parsed.
  inline std::size_t remaining() const noexcept
  {
    return (pos < tokens.size()) ? tokens.size() - pos : 0;
  }
protected:
  /// Parses for common data found in stroke node derived classes.
  /// This is meant to be called in a loop that parses the derived class.
  ///
  /// @param node A reference to the node that should be assigned the parsed data.
  ///
  /// @return True on a match, false on no match.
  /// False does not indicate an error occurred.
  bool parseStrokeNode(StrokeNode& node)
  {
    auto pixelSize = parseInt("pixel_size");
    if (pixelSize.valid) {
      node.pixelSize = safePixelSize(pixelSize.value);
      return true;
    }

    auto color = parseColor("color");
    if (color.valid) {
      node.color = color.value;
      return true;
    }

    return false;
  }
  /// Parses a list of vertices.
  ///
  /// @param name The name of the vertices.
  /// @param vertices The array to put the vertices into.
  ///
  /// @return True on success, false on failure.
  bool parseVertices(const char* name, std::vector<Vec2>& vertices)
  {
    if (!matchID(name)) {
      return false;
    }

    while (remaining() && !failed()) {

      if (matchID("end")) {
        break;
      }

      auto v = parseVector<2>();
      if (!v.valid) {
        formatError(look()) << "Failed to parse vector";
        return false;
      } else {
        vertices.emplace_back(v.value);
      }
    }

    return true;
  }
  /// Parses for a set list of vertices.
  bool parseVertices(const char* name, Vec2* vertices, std::size_t count) noexcept
  {
    auto firstTok = look();

    if (!matchID(name)) {
      return false;
    }

    for (std::size_t i = 0; i < count; i++) {

      auto v = parseVector<2>();
      if (!v.valid) {
        formatError(firstTok) << "Failed to match point " << i;
        return false;
      }

      vertices[i] = v.value;
    }

    return true;
  }
  /// Attempts to parse a fill node.
  NodePtr parseFillNode()
  {
    auto firstTok = look();

    if (!matchID("fill")) {
      return NodePtr();
    }

    Fill fill;

    while (remaining() && !failed()) {

      auto c = parseColor("color");
      if (c.valid) {
        fill.color = c.value;
        continue;
      }

      auto v = parseVector<2>("origin");
      if (v.valid) {
        fill.origin = v.value;
        continue;
      }

      if (matchID("end")) {
        break;
      } else if (failed()) {
        return NodePtr();
      } else {
        formatError(firstTok) << "Missing 'end' statement";
        return NodePtr();
      }
    }

    if (failed()) {
      return NodePtr();
    } else {
      return NodePtr(new Fill(std::move(fill)));
    }
  }
  /// Attempts to parse an ellipse node.
  NodePtr parseEllipseNode()
  {
    auto firstTok = look();

    if (!matchID("ellipse")) {
      return NodePtr();
    }

    Ellipse ellipse;

    while (remaining() && !failed()) {

      if (parseStrokeNode(ellipse)) {
        continue;
      }

      auto v = parseVector<2>("center");
      if (v.valid) {
        ellipse.center = v.value;
        continue;
      }

      v = parseVector<2>("radius");
      if (v.valid) {
        ellipse.radius = v.value;
        continue;
      }

      if (matchID("end")) {
        break;
      } else if (failed()) {
        break;
      } else {
        formatError(firstTok) << "Missing 'end' statement.";
        return NodePtr();
      }
    }

    if (failed()) {
      return NodePtr();
    } else {
      return NodePtr(new Ellipse(std::move(ellipse)));
    }
  }
  /// Attempts to parse a line node.
  NodePtr parseLineNode()
  {
    auto firstTok = look();

    if (!matchID("line")) {
      return NodePtr();
    }

    Line line;

    while (remaining() && !failed() && !matchID("end")) {

      if (parseStrokeNode(line)) {
        continue;
      }

      if (parseVertices("points", line.points)) {
        continue;
      }

      if (!failed()) {
        formatError(firstTok) << "Missing 'end' statement.";
        return NodePtr();
      }
    }

    if (failed()) {
      return NodePtr();
    } else {
      return NodePtr(new Line(std::move(line)));
    }
  }
  /// Parses for a quadrilateral node.
  NodePtr parseQuadNode()
  {
    auto firstTok = look();

    if (!matchID("quad")) {
      return NodePtr();
    }

    Quad quad;

    while (remaining() && !failed() && !matchID("end")) {

      if (parseStrokeNode(quad)) {
        continue;
      }

      if (parseVertices("points", quad.points, 4)) {
        continue;
      }

      if (!failed()) {
        formatError(firstTok) << "Missing 'end' statement.";
        return NodePtr();
      }
    }

    if (failed()) {
      return NodePtr();
    } else {
      return NodePtr(new Quad(std::move(quad)));
    }
  }
  /// Converts an integer vector to a color value.
  Color toColor(const Vector<int, 4>& v)
  {
    return Color {
      float(v[0]) / colorRes(),
      float(v[1]) / colorRes(),
      float(v[2]) / colorRes(),
      float(v[3]) / colorRes()
    };
  }
  /// Attempts to match a name.
  /// If a name is matched, then the
  /// parser is moved passed its position.
  ///
  /// @param name The name to match.
  ///
  /// @return True if the name was found,
  /// false if it was not.
  bool matchID(const char* name) noexcept
  {
    auto nameToken = look();
    if (nameToken != TokenType::Identifier) {
      return false;
    }

    if (nameToken != name) {
      return false;
    }

    next();

    return true;
  }
  /// Parses a decimal integer from a string.
  ///
  /// @param tok The token containing the string to parse.
  ///
  /// @return On success, a valid integer instance is returned.
  /// On failure, an optional container that evaluates to zero.
  Optional<int> parseInt(const Token& tok) noexcept
  {
    const char* str = tok.data;
    std::size_t s = tok.size;

    if (!s) {
      return Optional<int>();
    }

    std::size_t pos = 0;

    bool neg = (str[0] == '-');
    if (neg) {
      pos++;
    }

    int value = 0;

    auto isDec = [](char c) { return (c >= '0') && (c <= '9'); };

    while (pos < s) {

      if (!isDec(str[pos])) {
        formatError(tok) << "Non-decimal character found in " << tok;
        break;
      }

      value *= 10;
      value += int(str[pos] - '0');

      pos++;
    }

    return neg ? -value : value;
  }
  /// Gets a previous token that the parser
  /// has moved passed.
  Token previousTok(std::size_t offset = 1) const noexcept
  {
    if (offset > pos) {
      return Token();
    }

    return tokens[pos - offset];
  }
  /// Goes to the next number of tokens.
  ///
  /// @param count The number of tokens to go passed.
  void next(std::size_t count = 1) noexcept
  {
    pos += count;
  }
  /// Gets a token at a certain offset.
  Token look(std::size_t offset = 0) const noexcept
  {
    if (inBounds(offset)) {
      return tokens[pos + offset];
    } else {
      return Token();
    }
  }
  /// Indicates if a certain offset is in bounds.
  inline bool inBounds(std::size_t offset) const noexcept
  {
    return (pos + offset) < tokens.size();
  }
  /// Emits an error indicating that an internal parser
  /// error has occurred.
  void parserError(const Token& t, int line)
  {
    auto& stream = formatError(t);
    stream << "Internal parser error occurred (";
    stream << __FILE__;
    stream << ':';
    stream << line;
    stream << "). Please report an issue!";
  }
  /// Creates an error and returns it for formatting.
  ///
  /// @param t The token that caused the error.
  /// The position information is used to indicate
  /// where the error occurred.
  ///
  /// @return A reference to the stream that can be
  /// used to format the error.
  std::ostream& formatError(const Token& t)
  {
    if (failedFlag) {
      return suppressed;
    }

    failedFlag = true;

    Error error {
      std::ostringstream(),
      std::string(),
      t.line,
      t.column,
      t.pos,
      t.size
    };

    errorList.errors.emplace_back(std::move(error));

    return errorList.errors[errorList.errors.size() - 1].stream;
  }
};

} // namespace

//===================//
// Section: Document //
//===================//

/// Contains the implementation data of the document class.
struct Document final
{
  /// The layers of the document, each containing the
  /// draw operations to be completed by the painer.
  /// The first layer is returned first and is therefore
  /// the "bottom" layer.
  std::vector<LayerPtr> layers;
  /// The width of the document, in pixels.
  std::size_t width = 64;
  /// The height of the document, in pixels.
  std::size_t height = 64;
  /// The default background color.
  Color background = transparent();
  /// Makes a new document.
  Document()
  {
    addLayer(this);
  }
  /// Makes a copy of the document.
  ///
  /// @param other The document to copy.
  Document(const Document& other)
  {
    width = other.width;
    height = other.height;
    background = other.background;

    for (const auto& otherLayer : other.layers) {
      layers.emplace_back(new Layer(*otherLayer));
    }
  }
  /// Resets back to initial state.
  void reset()
  {
    *this = Document();
  }
  /// Moves a document to a new variable
  /// via move semantics.
  Document& operator = (Document&& other)
  {
    layers = std::move(other.layers);
    width = other.width;
    height = other.height;
    background = other.background;
    return *this;
  }
};

namespace {

/// Indicates if a layer name exists already.
/// This is used when automatically naming layers,
/// so that the automatically generated layer has a
/// unique name (even though it doesn't need one.)
///
/// @param doc The document to check the layer names for.
/// @param name The name to search for.
///
/// @return True if the name exists, false if it does not.
bool layerNameExists(const Document* doc, const char* name) noexcept
{
  for (const auto& l : doc->layers) {
    if (l->name == name) {
      return true;
    }
  }

  return false;
}

/// Formats a layer name.
///
/// @param index The index to put into the layer name.
///
/// @return The formatted layer name.
std::string formatLayerName(std::size_t index)
{
  std::stringstream stream;
  stream << "Layer ";
  stream << index;
  return stream.str();
}

/// Generates a unique layer name.
///
/// @param doc The document to generate the layer name for.
///
/// @return The unique layer name.
std::string uniqueLayerName(const Document* doc)
{
  for (std::size_t i = doc->layers.size() + 1; i < SIZE_MAX; i++) {

    auto name = formatLayerName(i);
    if (!layerNameExists(doc, name.c_str())) {
      return name;
    }
  }

  // It would be odd if this was reached.

  return "";
}

} // namespace

Document* createDoc()
{
  return new Document();
}

void closeDoc(Document* doc) noexcept
{
  delete doc;
}

Document* copyDoc(const Document* doc)
{
  return new Document(*doc);
}

bool openDoc(Document* doc, const char* filename, ErrorList** errListPtr)
{
  if (!filename) {
    return false;
  }

  std::ifstream file(filename);
  if (!file.good()) {
    return false;
  }

  std::stringstream buf;

  buf << file.rdbuf();

  std::string content(buf.str());

  Parser parser(content.data(), content.size());

  while (parser.remaining() && !parser.failed()) {

    auto w = parser.parseSize("width");
    if (w.valid) {
      doc->width = w.value;
      continue;
    } else if (parser.failed()) {
      break;
    }

    auto h = parser.parseSize("height");
    if (h.valid) {
      doc->height = h.value;
      continue;
    } else if (parser.failed()) {
      break;
    }

    auto bg = parser.parseColor("background");
    if (bg.valid) {
      doc->background = bg.value;
      continue;
    } else if (parser.failed()) {
      break;
    }

    auto node = parser.parseNode();
    if (node) {
      if (doc->layers.empty()) {
        addLayer(doc);
      }
      doc->layers[0]->nodes.emplace_back(std::move(node));
      continue;
    } else if (parser.failed()) {
      break;
    }

    auto layer = parser.parseLayer();
    if (layer) {
      doc->layers.emplace_back(std::move(layer));
      continue;
    } else if (parser.failed()) {
      break;
    }

    parser.badToken();
    break;
  }

  if (parser.failed()) {

    if (errListPtr) {
      *errListPtr = parser.getErrorList(filename, std::move(content));
    }

    return false;
  }

  return true;
}

bool saveDoc(const Document* doc, const char* filename)
{
  std::ofstream file(filename);
  if (!file.good()) {
    return false;
  }

  Encoder encoder(file);

  encoder.encodeSize("width", doc->width);
  encoder.encodeSize("height", doc->height);
  encoder.encodeColor("background", doc->background);

  for (const auto& layer : doc->layers) {
    encoder.encodeLayer(*layer);
  }

  return true;
}

Layer* addLayer(Document* doc)
{
  LayerPtr layer(new Layer());

  layer->name = uniqueLayerName(doc);

  doc->layers.emplace_back(layer.get());

  return layer.release();
}

void removeLayer(Document* doc, std::size_t layer)
{
  // std::vector::erase causes UB if the index
  // is out of bounds, so we have to manually check
  // and throw an exception if that's the case.

  if (layer >= doc->layers.size()) {
    throw std::out_of_range("Layer index is out of range");
  }

  doc->layers.erase(doc->layers.begin() + layer);
}

Layer* getLayer(Document* doc, std::size_t layer)
{
  return doc->layers.at(layer).get();
}

const Layer* getLayer(const Document* doc, std::size_t layer)
{
  return doc->layers.at(layer).get();
}

std::size_t getLayerCount(const Document* doc) noexcept
{
  return doc->layers.size();
}

void moveLayer(Document* doc, std::size_t src, std::size_t dst)
{
  LayerPtr tmp = std::move(doc->layers.at(src));

  for (std::size_t i = src; i < dst; i++) {
    doc->layers.at(i) = std::move(doc->layers.at(i + 1));
  }

  for (std::size_t i = src; i > dst; i--) {
    doc->layers.at(i) = std::move(doc->layers.at(i - 1));
  }

  doc->layers.at(dst) = std::move(tmp);
}

Ellipse* addEllipse(Document* doc, std::size_t layer)
{
  return doc->layers.at(layer)->addNode(new Ellipse());
}

Fill* addFill(Document* doc, std::size_t layer)
{
  return doc->layers.at(layer)->addNode(new Fill());
}

Line* addLine(Document* doc, std::size_t layer)
{
  return doc->layers.at(layer)->addNode(new Line());
}

Quad* addQuad(Document* doc, std::size_t layer)
{
  return doc->layers.at(layer)->addNode(new Quad());
}

std::size_t getDocWidth(const Document* doc) noexcept { return doc->width; }

std::size_t getDocHeight(const Document* doc) noexcept { return doc->height; }

void getBackground(const Document* doc, float* bg) noexcept
{
  bg[0] = doc->background[0];
  bg[1] = doc->background[1];
  bg[2] = doc->background[2];
  bg[3] = doc->background[3];
}

void resizeDoc(Document* doc, std::size_t width, std::size_t height) noexcept
{
  doc->width = width;
  doc->height = height;
}

void setBackground(Document* doc, float r, float g, float b, float a) noexcept
{
  doc->background = clip(Color { r, g, b, a });
}

//============================//
// Section: Render Algorithms //
//============================//

/// This function is based on the algorithm
/// described by John Kennedy on rasterizing
/// an ellipse.
///
/// @param cx The center X component.
/// @param cy The center Y component.
/// @param xRadius The X radius
/// @param yRadius The Y radius
/// @param functor Receives the points to plot on the ellipse.
template <typename Functor>
void renderEllipse(int cx, int cy, int xRadius, int yRadius, Functor functor) noexcept
{
  if (!xRadius || !yRadius) {
    return;
  }

  auto plot4 = [cx, cy, functor](int x, int y) {
    functor(cx + x, cy + y);
    functor(cx - x, cy + y);
    functor(cx - x, cy - y);
    functor(cx + x, cy - y);
  };

  int twoASquare = 2 * xRadius * xRadius;
  int twoBSquare = 2 * yRadius * yRadius;

  int x = xRadius;
  int y = 0;

  int xChange = yRadius * yRadius * (1 - (2 * xRadius));
  int yChange = xRadius * xRadius;

  int ellipseError = 0;

  int stoppingX = twoBSquare * xRadius;
  int stoppingY = 0;

  while (stoppingX >= stoppingY) {

    plot4(x, y);

    y++;
    stoppingY += twoASquare;
    ellipseError += yChange;
    yChange += twoASquare;

    if (((2 * ellipseError) + xChange) > 0) {
      x--;
      stoppingX -= twoBSquare;
      ellipseError += xChange;
      xChange += twoBSquare;
    }
  }

  x = 0;
  y = yRadius;

  xChange = yRadius * yRadius;
  yChange = xRadius * xRadius * (1 - (2 * yRadius));

  ellipseError = 0;

  stoppingX = 0;
  stoppingY = twoASquare * yRadius;

  while (stoppingX <= stoppingY) {

    plot4(x, y);

    x++;
    stoppingX += twoBSquare;
    ellipseError += xChange;
    xChange += twoBSquare;
    if (((2 * ellipseError) + yChange) > 0) {
      y--;
      stoppingY -= twoASquare;
      ellipseError += yChange;
      yChange += twoASquare;
    }
  }

  // 1st point set done
}

//==================//
// Section: Painter //
//==================//

/// Used for rasterizing the document.
class Painter final : public NodeAccessor
{
  /// The current pixel size.
  std::size_t pixelSize = 1;
  /// The current material used to paint with.
  Color primaryColor = Color { 0, 0, 0, 0 };
  /// The color buffer being rendered to.
  float* colorBuffer = nullptr;
  /// The width of the color buffer, in pixels.
  std::size_t width = 0;
  /// The height of the color buffer, in pixels.
  std::size_t height = 0;
public:
  Painter(float* c, std::size_t w, std::size_t h) : colorBuffer(c), width(w), height(h) {}
  /// Renders an ellipse.
  void access(const Ellipse& ellipse) noexcept override
  {
    primaryColor = ellipse.color;

    pixelSize = ellipse.pixelSize;

    auto functor = [this] (int x, int y) {
      plot(x, y);
    };

    renderEllipse(ellipse.center[0],
                  ellipse.center[1],
                  ellipse.radius[0],
                  ellipse.radius[1],
                  functor);
  }
  /// Fills an area on the image
  /// with a certain color.
  void access(const Fill& fill) noexcept override
  {
    if (!inBounds(fill.origin)) {
      return;
    }

    auto prev = getPixel(fill.origin);

    if (almostEqual(prev, fill.color)) {
      return;
    }

    try {
      this->fill(fill.origin, prev, fill.color);
    } catch (...) { }
  }
  /// Renders a line.
  void access(const Line& line) noexcept override
  {
    primaryColor = line.color;
    pixelSize = line.pixelSize;

    for (std::size_t i = 1; i < line.points.size(); i++) {
      drawLine(line.points[i - 1], line.points[i - 0]);
    }
  }
  /// Draws a quadrilateral.
  void access(const Quad& quad) noexcept override
  {
    primaryColor = quad.color;

    pixelSize = quad.pixelSize;

    drawLine(quad.points[0], quad.points[1]);
    drawLine(quad.points[1], quad.points[2]);
    drawLine(quad.points[2], quad.points[3]);
    drawLine(quad.points[3], quad.points[0]);
  }
  /// Clears the contents of the color buffer.
  void clear(const Color& c) noexcept
  {
    unsigned max = width * height * 4;

    for (unsigned i = 0; i < max; i += 4) {
      colorBuffer[i + 0] = c[0];
      colorBuffer[i + 1] = c[1];
      colorBuffer[i + 2] = c[2];
      colorBuffer[i + 3] = c[3];
    }
  }
  void drawLine(const Vec2& a, const Vec2& b) noexcept
  {
    auto diff = absolute(a - b);

    diff[1] = -diff[1];

    int signX = (a[0] < b[0]) ? 1 : -1;
    int signY = (a[1] < b[1]) ? 1 : -1;

    int err = diff[0] + diff[1];

    auto p = a;

    for (;;) {

      plot(p[0], p[1]);

      if (p == b) {
        break;
      }

      int err2 = 2 * err;

      if (err2 >= diff[1]) {
        err += diff[1];
        p[0] += signX;
      }

      if (err2 <= diff[0]) {
        err += diff[0];
        p[1] += signY;
      }
    }
  }
  /// Plots a point onto the color buffer.
  ///
  /// @param x The X coordinate of the point to plot.
  /// @param y The Y coordinate of the point to plot.
  inline void plot(int x, int y) { plot(Vec2 { x, y }); }
  /// Plots a point onto the color buffer.
  ///
  /// @param p The point to plot within the color buffer.
  void plot(const Vec2& p)
  {
    Vec2 min { p - (pixelSize - 1) };
    Vec2 max = p;

    const Vec2 imageMin = Vec2 { 0, 0 };
    const Vec2 imageMax = Vec2 { int(width), int(height) };

    min = clip(min, imageMin, imageMax);
    max = clip(max, imageMin, imageMax);

    for (int y = min[1]; y <= max[1]; y++) {
      for (int x = min[0]; x <= max[0]; x++) {
        setPixel(x, y, primaryColor);
      }
    }
  }
  /// Sets the value of a pixel.
  ///
  /// @note This function does not perform bounds checking.
  ///
  /// @param x The X coordinate of the pixel to set.
  /// @param y The Y coordinate of the pixel to set.
  /// @param c The color to assign the pixel.
  inline void setPixel(int x, int y, const Color& c)
  {
    auto* dst = &colorBuffer[((y * width) + x) * 4];
    dst[0] = c[0];
    dst[1] = c[1];
    dst[2] = c[2];
    dst[3] = c[3];
  }
  /// Sets the value of a pixel.
  ///
  /// @note This function does not perform bounds checking.
  ///
  /// @param p The position of the pixel to set.
  /// @param c The color to assign the pixel.
  inline void setPixel(const Vec2& p, const Color& c)
  {
    return setPixel(p[0], p[1], c);
  }
  /// Gets the color from a pixel at a certain point.
  ///
  /// @note This function does not perform bounds checking.
  ///
  /// @param p The point to get the pixel color from.
  ///
  /// @return The color at the specified point.
  inline Color getPixel(const Vec2& p) const noexcept
  {
    return getPixel(p[0], p[1]);
  }
  /// Gets the color from a pixel at a certain point.
  ///
  /// @note This function does not perform bounds checking.
  ///
  /// @param x The X coordinate of the pixel to get.
  /// @param y The Y coordinate of the pixel to get.
  ///
  /// @return The color at the specified point.
  inline Color getPixel(int x, int y) const noexcept
  {
    const float* src = &colorBuffer[((y * width) + x) * 4];

    return Color { src[0], src[1], src[2], src[3] };
  }
  /// Indicates if a point is in bounds or not.
  ///
  /// @param p The point to check.
  ///
  /// @return True on success, false on failure.
  inline bool inBounds(const Vec2& p) const noexcept
  {
    return ((p[0] >= 0) && (std::size_t(p[0]) < width))
        && ((p[1] >= 0) && (std::size_t(p[1]) < height));
  }
protected:
  /// Fills an area on the image with a color.
  ///
  /// @param origin The point to start at.
  ///
  /// @param prev The previous color.
  /// @param next The color to be assigned.
  void fill(const Vec2& origin, const Color& prev, const Color& next)
  {
    if (!inBounds(origin)) {
      return;
    }

    int xMax = int(width);
    int yMax = int(height);

    std::vector<Vec2> stack;

    stack.push_back(origin);

    auto pop = [](std::vector<Vec2>& stk) {
      auto poppedItem = stk[stk.size() - 1];
      stk.pop_back();
      return poppedItem;
    };

    while (!stack.empty()) {

      auto p = pop(stack);

      auto x1 = p[0];

      while ((x1 >= 0) && almostEqual(getPixel(x1, p[1]), prev)) {
        x1--;
      }

      x1++;

      auto spanAbove = false;
      auto spanBelow = false;

      while ((x1 >= 0) && (x1 < xMax) && almostEqual(getPixel(x1, p[1]), prev)) {

        setPixel(x1, p[1], next);

        if (!spanAbove && (p[1] > 0) && almostEqual(getPixel(x1, p[1] - 1), prev)) {
          stack.push_back(Vec2 { x1, p[1] - 1 });
          spanAbove = true;
        } else if (spanAbove && (p[1] > 0) && !almostEqual(getPixel(x1, p[1] - 1), prev)) {
          spanAbove = false;
        }

        if (!spanBelow && (p[1] < (yMax - 1)) && almostEqual(getPixel(x1, p[1] + 1), prev)) {
          stack.push_back(Vec2 { x1, p[1] + 1 });
          spanBelow = true;
        } else if (spanBelow && (p[1] < (yMax - 1)) && !almostEqual(getPixel(x1, p[1] + 1), prev)) {
          spanBelow = false;
        }

        x1++;
      }
    }
  }
};

void render(const Document* doc, float* colorBuffer, std::size_t w, std::size_t h) noexcept
{
  Painter painter(colorBuffer, w, h);

  painter.clear(doc->background);

  for (const auto& layer : doc->layers) {
    for (const auto& node : layer->nodes) {
      node->accept(painter);
    }
  }
}

void render(const Document* doc, Image* image) noexcept
{
  render(doc, image->colorBuffer.data(), image->width, image->height);
}

} // namespace px
