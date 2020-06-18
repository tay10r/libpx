#ifndef LIBPX_EDITOR_DRAW_MODE_HPP
#define LIBPX_EDITOR_DRAW_MODE_HPP

#include "Mode.hpp"

#include <libpx.hpp>

#include <cstddef>

namespace px {

struct Document;

class Editor;

class DrawMode : public Mode
{
public:
  virtual ~DrawMode() {}

  virtual Document* getDocument() = 0;
  virtual const Document* getDocument() const noexcept = 0;

  virtual Editor* getEditor() noexcept = 0;
  virtual const Editor* getEditor() const noexcept = 0;

  virtual BlendMode getBlendMode() const noexcept = 0;

  virtual int getPixelSize() const noexcept = 0;

  virtual float* getPrimaryColor() noexcept = 0;
  virtual const float* getPrimaryColor() const noexcept = 0;

  virtual const int* getCursor() const noexcept = 0;

  virtual std::size_t requireCurrentLayer() = 0;
};

/// Creates a new draw mode instance.
///
/// @param editor A pointer to the editor interface.
///
/// @return A pointer to the draw mode.
DrawMode* createDrawMode(Editor* editor);

} // namespace px

#endif // LIBPX_EDITOR_DRAW_MODE_HPP
