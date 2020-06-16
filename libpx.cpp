#include "libpx.hpp"

#include <memory>
#include <vector>

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

//======================//
// Section: Scene Graph //
//======================//

namespace {

constexpr Color white() noexcept { return Color { 1, 1, 1, 1 }; }
constexpr Color black() noexcept { return Color { 0, 0, 0, 1 }; }

class NodeAccessor
{
public:
  virtual ~NodeAccessor() {}
  virtual void access(const Line& line) noexcept = 0;
  virtual void access(const Fill& fill) noexcept = 0;
};

struct Node
{
  virtual ~Node() {}

  virtual void accept(NodeAccessor& accessor) const noexcept = 0;
};

} // namespace

struct Line final : public Node
{
  Color color = black();
  std::size_t pixelSize = 1;
  std::vector<Vec2> points;

  void accept(NodeAccessor& accessor) const noexcept override
  {
    accessor.access(*this);
  }
};

bool addPoint(Line* line, int x, int y) noexcept
{
  try {
    line->points.emplace_back(Vec2 { x, y });
  } catch (...) {
    return false;
  }

  return true;
}

bool setPoint(Line* line, std::size_t index, int x, int y) noexcept
{
  if (index >= line->points.size()) {
    return false;
  }

  line->points[index] = Vec2 { x, y };

  return true;
}

void setPixelSize(Line* line, int pixelSize) noexcept
{
  line->pixelSize = pixelSize;
}

void setLineColor(Line* line, float r, float g, float b, float a) noexcept
{
  line->color = Color { r, g, b, a };
}

struct Fill final : public Node
{
  Color color = black();
  Vec2 origin = Vec2 { 0, 0 };

  void accept(NodeAccessor& accessor) const noexcept override
  {
    accessor.access(*this);
  }
};

void setFillOrigin(Fill* fill, int x, int y) noexcept
{
  fill->origin = Vec2 { x, y };
}

void setFillColor(Fill* fill, float r, float g, float b, float a) noexcept
{
  fill->color = Color { r, g, b, a };
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

Image* createImage(std::size_t width, std::size_t height) noexcept
{
  Image* image = nullptr;

  try {
    image = new Image();
  } catch (...) {
    return nullptr;
  }

  try {
    resizeImage(image, width, height);
  } catch (...) {
    delete image;
    return nullptr;
  }

  return image;
}

void closeImage(Image* image) noexcept
{
  delete image;
}

const float* getColorBuffer(const Image* image) noexcept
{
  return image->colorBuffer.data();
}

std::size_t getImageWidth(const Image* image) noexcept { return image->width; }

std::size_t getImageHeight(const Image* image) noexcept { return image->height; }

bool resizeImage(Image* image, std::size_t w, std::size_t h) noexcept
{
  try {
    image->colorBuffer.resize(w * h * 4);
  } catch (...) {
    return false;
  }

  image->width = w;
  image->height = h;

  return true;
}

//===================//
// Section: Document //
//===================//

/// Contains the implementation data of the document class.
struct Document final
{
  /// The nodes added to the document.
  std::vector<std::unique_ptr<Node>> nodes;
  /// The width of the document, in pixels.
  std::size_t width = 64;
  /// The height of the document, in pixels.
  std::size_t height = 64;
  /// The default background color.
  Color background = white();
};

Document* createDoc() noexcept
{
  try {
    return new Document();
  } catch (...) {
    return nullptr;
  }
}

void closeDoc(Document* doc) noexcept
{
  delete doc;
}

Line* addLine(Document* doc) noexcept
{
  Line* line = nullptr;

  try {
    line = new Line();
  } catch (...) {
    return nullptr;
  }

  try {
    doc->nodes.emplace_back(line);
  } catch (...) {
    delete line;
    return nullptr;
  }

  return line;
}

Fill* addFill(Document* doc) noexcept
{
  Fill* fill = nullptr;

  try {
    fill = new Fill();
  } catch (...) {
    return nullptr;
  }

  try {
    doc->nodes.emplace_back(fill);
  } catch (...) {
    delete fill;
    return nullptr;
  }

  return fill;
}

std::size_t getDocWidth(const Document* doc) noexcept { return doc->width; }

std::size_t getDocHeight(const Document* doc) noexcept { return doc->height; }

void resizeDoc(Document* doc, std::size_t width, std::size_t height) noexcept
{
  doc->width = width;
  doc->height = height;
}

void setBackground(Document* doc, float r, float g, float b, float a) noexcept
{
  doc->background = Color { r, g, b, a };
}

//==================//
// Section: Painter //
//==================//

/// Contains the implementation data of the default painter interface.
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
  /// Renders a line.
  void access(const Line& line) noexcept override
  {
    primaryColor = line.color;
    pixelSize = line.pixelSize;

    for (std::size_t i = 1; i < line.points.size(); i++) {
      drawLine(line.points[i - 1], line.points[i - 0]);
    }
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
    Vec2 min { p - pixelSize };
    Vec2 max { p + pixelSize };

    const Vec2 imageMin = Vec2 { 0, 0 };
    const Vec2 imageMax = Vec2 { int(width), int(height) };

    min = clip(min, imageMin, imageMax);
    max = clip(max, imageMin, imageMax);

    for (int y = min[1]; y < max[1]; y++) {
      for (int x = min[0]; x < max[0]; x++) {
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

  for (const auto& node : doc->nodes) {
    node->accept(painter);
  }
}

void render(const Document* doc, Image* image) noexcept
{
  render(doc, image->colorBuffer.data(), image->width, image->height);
}

} // namespace px
