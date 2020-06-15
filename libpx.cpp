#include "libpx.hpp"

#include <memory>
#include <vector>

namespace px {

namespace {

//===============//
// Section: Vec2 //
//===============//

struct Vec2 final
{
  Scalar x = 0;
  Scalar y = 0;
};

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

/// Adds a scalar value to a vector.
inline Vec2 operator + (const Vec2& a, Scalar b) noexcept
{
  return Vec2 { a.x + b, a.y + b };
}

/// Subtracts a scalar value to a vector.
inline Vec2 operator - (const Vec2& a, Scalar b) noexcept
{
  return Vec2 { a.x - b, a.y - b };
}

/// Adds two vectors.
inline Vec2 operator + (const Vec2& a, const Vec2& b) noexcept
{
  return Vec2 { b.x + a.x, b.y + a.y };
}

/// Subtracts two vectors.
inline Vec2 operator - (const Vec2& a, const Vec2& b) noexcept
{
  return Vec2 { b.x - a.x, b.y - a.y };
}

/// Indicates if two 2D vectors are equal.
inline bool operator == (const Vec2& a, const Vec2& b) noexcept
{
  return (a.x == b.x) && (a.y == b.y);
}

/// Calculates a vector with all absolute value components.
///
/// @param v The vector to get the absolute values of.
///
/// @return The resultant vector.
inline Vec2 absolute(const Vec2& v) noexcept
{
  return Vec2 {
    absolute(v.x),
    absolute(v.y)
  };
}

/// Calculates the minimum components of two 2D vectors.
inline Vec2 min(const Vec2& a, const Vec2& b) noexcept
{
  return Vec2 {
    std::min(a.x, b.x),
    std::min(a.y, b.y)
  };
}

/// Calculates the maximum components of two 2D vectors.
inline Vec2 max(const Vec2& a, const Vec2& b) noexcept
{
  return Vec2 {
    std::max(a.x, b.x),
    std::max(a.y, b.y)
  };
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
    std::min(std::max(in.x, min.x), max.x),
    std::min(std::max(in.y, min.y), max.y)
  };
}

} // namespace

//======================//
// Section: Scene Graph //
//======================//

namespace {

struct Color final
{
  Byte r = 0;
  Byte g = 0;
  Byte b = 0;
  Byte a = 255;

  inline bool operator != (const Color& other) noexcept
  {
    return (r != other.r)
        || (g != other.g)
        || (b != other.b)
        || (a != other.a);
  }
  inline bool operator == (const Color& other) noexcept
  {
    return (r == other.r)
        && (g == other.g)
        && (b == other.b)
        && (a == other.a);
  }
};

constexpr Color white() noexcept { return Color { 255, 255, 255, 255 }; }
constexpr Color black() noexcept { return Color {   0,   0,   0, 255 }; }

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
  Size pixelSize = 1;
  std::vector<Vec2> points;

  void accept(NodeAccessor& accessor) const noexcept override
  {
    accessor.access(*this);
  }
};

bool addPoint(Line* line, Scalar x, Scalar y) noexcept
{
  try {
    line->points.emplace_back(Vec2 { x, y });
  } catch (...) {
    return false;
  }

  return true;
}

void setPixelSize(Line* line, Scalar pixelSize) noexcept
{
  line->pixelSize = pixelSize;
}

void setLineColor(Line* line, Byte r, Byte g, Byte b, Byte a) noexcept
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

void setFillOrigin(Fill* fill, Scalar x, Scalar y) noexcept
{
  fill->origin.x = x;
  fill->origin.y = y;
}

void setFillColor(Fill* fill, Byte r, Byte g, Byte b, Byte a) noexcept
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
  std::vector<Byte> colorBuffer;
  /// The width of the image, in pixels.
  Size width = 0;
  /// The height of the image, in pixels.
  Size height = 0;
};

Image* createImage(Size width, Size height) noexcept
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

const Byte* getColorBuffer(const Image* image) noexcept
{
  return image->colorBuffer.data();
}

Size getImageWidth(const Image* image) noexcept { return image->width; }

Size getImageHeight(const Image* image) noexcept { return image->height; }

bool resizeImage(Image* image, Size w, Size h) noexcept
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
  Size width = 64;
  /// The height of the document, in pixels.
  Size height = 64;
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

Size getDocWidth(const Document* doc) noexcept { return doc->width; }

Size getDocHeight(const Document* doc) noexcept { return doc->height; }

void resizeDoc(Document* doc, Size width, Size height) noexcept
{
  doc->width = width;
  doc->height = height;
}

void setBackground(Document* doc, Byte r, Byte g, Byte b, Byte a) noexcept
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
  Size pixelSize = 1;
  /// The current material used to paint with.
  Color primaryColor = Color { 0, 0, 0, 0 };
  /// The color buffer being rendered to.
  Byte* colorBuffer;
  /// The width of the color buffer, in pixels.
  Size width = 0;
  /// The height of the color buffer, in pixels.
  Size height = 0;
public:
  Painter(Byte* c, Size w, Size h) : colorBuffer(c), width(w), height(h) {}
  /// Renders a line.
  void access(const Line& line) noexcept override
  {
    primaryColor = line.color;
    pixelSize = line.pixelSize;

    for (Size i = 1; i < line.points.size(); i++) {
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

    if (prev == fill.color) {
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
      colorBuffer[i + 0] = c.r;
      colorBuffer[i + 1] = c.g;
      colorBuffer[i + 2] = c.b;
      colorBuffer[i + 3] = c.a;
    }
  }
  void drawLine(const Vec2& a, const Vec2& b) noexcept
  {
    Vec2 diff {
       absolute(a.x - b.x),
      -absolute(a.y - b.y)
    };

    Scalar signX = (a.x < b.x) ? 1 : -1;
    Scalar signY = (a.y < b.y) ? 1 : -1;

    Scalar err = diff.x + diff.y;

    auto p = a;

    for (;;) {

      plot(p.x, p.y);

      if (p == b) {
        break;
      }

      Scalar err2 = 2 * err;

      if (err2 >= diff.y) {
        err += diff.y;
        p.x += signX;
      }

      if (err2 <= diff.x) {
        err += diff.x;
        p.y += signY;
      }
    }
  }
  /// Plots a point onto the color buffer.
  ///
  /// @param x The X coordinate of the point to plot.
  /// @param y The Y coordinate of the point to plot.
  inline void plot(Scalar x, Scalar y) { plot(Vec2 { x, y }); }
  /// Plots a point onto the color buffer.
  ///
  /// @param p The point to plot within the color buffer.
  void plot(const Vec2& p)
  {
    Vec2 min { p - pixelSize };
    Vec2 max { p + pixelSize };

    const Vec2 imageMin = Vec2 { 0, 0 };
    const Vec2 imageMax = Vec2 { Scalar(width), Scalar(height) };

    min = clip(min, imageMin, imageMax);
    max = clip(max, imageMin, imageMax);

    for (Scalar y = min.y; y < max.y; y++) {
      for (Scalar x = min.x; x < max.x; x++) {
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
  inline void setPixel(Scalar x, Scalar y, const Color& c)
  {
    auto* dst = &colorBuffer[((y * width) + x) * 4];
    dst[0] = c.r;
    dst[1] = c.g;
    dst[2] = c.b;
    dst[3] = c.a;
  }
  /// Sets the value of a pixel.
  ///
  /// @note This function does not perform bounds checking.
  ///
  /// @param p The position of the pixel to set.
  /// @param c The color to assign the pixel.
  inline void setPixel(const Vec2& p, const Color& c)
  {
    return setPixel(p.x, p.y, c);
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
    return getPixel(p.x, p.y);
  }
  /// Gets the color from a pixel at a certain point.
  ///
  /// @note This function does not perform bounds checking.
  ///
  /// @param x The X coordinate of the pixel to get.
  /// @param y The Y coordinate of the pixel to get.
  ///
  /// @return The color at the specified point.
  inline Color getPixel(Scalar x, Scalar y) const noexcept
  {
    const Byte* src = &colorBuffer[((y * width) + x) * 4];

    return Color { src[0], src[1], src[2], src[3] };
  }
  /// Indicates if a point is in bounds or not.
  ///
  /// @param p The point to check.
  ///
  /// @return True on success, false on failure.
  inline bool inBounds(const Vec2& p) const noexcept
  {
    return ((p.x >= 0) && (Size(p.x) < width))
        && ((p.y >= 0) && (Size(p.y) < height));
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

    Scalar xMax = Scalar(width);
    Scalar yMax = Scalar(height);

    std::vector<Vec2> stack;

    stack.push_back(origin);

    auto pop = [](std::vector<Vec2>& stk) {
      auto last = stk[stk.size() - 1];
      stk.pop_back();
      return last;
    };

    while (!stack.empty()) {

      auto p = pop(stack);

      auto x1 = p.x;

      while ((x1 >= 0) && (getPixel(x1, p.y) == prev)) {
        x1--;
      }

      x1++;

      auto spanAbove = false;
      auto spanBelow = false;

      while ((x1 >= 0) && (x1 < xMax) && (getPixel(x1, p.y) == prev)) {

        setPixel(x1, p.y, next);

        if (!spanAbove && (p.y > 0) && (getPixel(x1, p.y - 1) == prev)) {
          stack.push_back(Vec2 { x1, p.y - 1 });
          spanAbove = true;
        } else if (spanAbove && (p.y > 0) && (getPixel(x1, p.y - 1) != prev)) {
          spanAbove = false;
        }

        if (!spanBelow && (p.y < (yMax - 1)) && (getPixel(x1, p.y + 1) == prev)) {
          stack.push_back(Vec2 { x1, p.y + 1 });
          spanBelow = true;
        } else if (spanBelow && (p.y < (yMax - 1)) && (getPixel(x1, p.y + 1) != prev)) {
          spanBelow = false;
        }

        x1++;
      }
    }
  }
};

void render(const Document* doc, Byte* colorBuffer, Size w, Size h) noexcept
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
