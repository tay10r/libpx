#ifndef LIBPX_EDITOR_GL_RENDERER_HPP
#define LIBPX_EDITOR_GL_RENDERER_HPP

#ifndef GL_GLEXT_PROTOTYPES
#define GL_GLEXT_PROTOTYPES
#endif // GL_GLEXT_PROTOTYPES

#include <GL/gl.h>

#include <cstddef>

#include "Renderer.hpp"

namespace px {

/// Used for rendering the document image buffer
/// to the window.
///
/// @note This does not handle the rendering of
/// the document geometry.
class GlRenderer final : public Renderer
{
  /// The vertex array ID.
  GLuint vertexArray = 0;
  /// Stores indices for the two triangles.
  GLuint elementBuffer = 0;
  /// The quadrilateral vertex buffer.
  GLuint vertexBuffer = 0;
  /// An index to the texture coordinate buffer.
  GLuint texCoordBuffer = 0;
  /// The vertex shader ID.
  GLuint vertexShader = 0;
  /// The fragment shader ID.
  GLuint fragmentShader = 0;
  /// The ID of the texture that
  /// receives the painter output.
  GLuint texture = 0;
  /// The GL program ID.
  GLuint program = 0;
  /// The location of the image transformation.
  GLuint transformLocation = 0;
  /// The base color of the checkerboard background.
  GLuint checkerboardColorLocation = 0;
  /// The location of the cursor position variable.
  GLuint cursorPosLocation = 0;
public:
  /// Releases the data allocated by the renderer.
  ~GlRenderer();
  /// Initializes the renderer.
  ///
  /// @return True on success, false on failure.
  bool init();
  /// Renders an image to the to the window.
  ///
  /// @param img The image to render.
  /// The image should be in the format of RGBA.
  /// @param w The width of the image, in pixels.
  /// @param h The height of the image, in pixels.
  void blit(const float* img, std::size_t w, std::size_t h) override;
  /// Clears the background.
  void clear(float r, float g, float b, float a) override;
  /// Sets the base color of the checkerboard pattern.
  void setCheckerboardColor(float r, float g, float b, float a) override;
  /// Sets the checkerboard contrast.
  void setCheckerboardContrast(float contrast) override;
  /// Updates the cursor position shown by the renderer.
  ///
  /// @param x The X coordinate of the cursor.
  /// @param y The Y coordinate of the cursor.
  void setCursor(int x, int y) override;
  /// Sets the translation values.
  void setTranslation(float x, float y) override;
  /// Sets the zoom factor.
  void setZoom(float zoom) override;
protected:
  /// Initializes a shader.
  ///
  /// @param name The name of the shader being initialized.
  /// This is used for debugging purposes, if the shader fails to compile.
  /// @param source The shader source code to compile.
  /// @param shaderType The type of shader being compiled.
  ///
  /// @return On success, a non-zero shader ID is returned.
  /// On failure, zero is returned.
  GLuint setupShader(const char* name, const char* source, GLenum shaderType);
};

} // namespace px

#endif // LIBPX_EDITOR_GL_RENDERER_HPP
