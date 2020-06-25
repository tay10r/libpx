#include "GlRenderer.hpp"

#ifdef PXEDIT_DESKTOP
#include "GlslDesktopShaders.hpp"
#endif

#ifdef PXEDIT_BROWSER
#include "GlslBrowserShaders.hpp"
#endif

#include <cstdio>
#include <cstdlib>

namespace px {

GlRenderer::~GlRenderer()
{
}

bool GlRenderer::init()
{
  glGenTextures(1, &texture);

  glBindTexture(GL_TEXTURE_2D, texture);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glGenVertexArrays(1, &vertexArray);
  glBindVertexArray(vertexArray);

  glGenBuffers(1, &vertexBuffer);
  glGenBuffers(1, &elementBuffer);

  const float vertices[] {
    // position      | texture coordinates
    1.0f,  1.0f, 0.0f, 1.0f, 0.0f,
    1.0f, -1.0f, 0.0f, 1.0f, 1.0f,
   -1.0f, -1.0f, 0.0f, 0.0f, 1.0f,
   -1.0f,  1.0f, 0.0f, 0.0f, 0.0f
  };

  const unsigned int indices[] {
    0, 1, 3,
    1, 2, 3
  };

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);

#ifdef PXEDIT_DESKTOP
  const char* vShaderSource = px::desktop::vertexShader;
  const char* fShaderSource = px::desktop::fragmentShader;
#endif

#ifdef PXEDIT_BROWSER
  const char* vShaderSource = px::browser::vertexShader;
  const char* fShaderSource = px::browser::fragmentShader;
#endif

  vertexShader = setupShader("Vertex Shader", vShaderSource, GL_VERTEX_SHADER);
  if (!vertexShader) {
    return false;
  }

  fragmentShader = setupShader("Fragment Shader", fShaderSource, GL_FRAGMENT_SHADER);
  if (!fragmentShader) {
    return false;
  }

  program = glCreateProgram();
  glAttachShader(program, vertexShader);
  glAttachShader(program, fragmentShader);
  glLinkProgram(program);

  GLint linkSuccess = GL_FALSE;

  glGetProgramiv(program, GL_LINK_STATUS, &linkSuccess);
  if (!linkSuccess) {
    return false;
  }
  // glDetachShader(programID, vertexShader);
  // glDetachShader(programID, fragmentShader);
  // glDeleteShader(vertexShader);
  // glDeleteShader(fragmentShader);

  glUseProgram(program);

  transformLocation            = glGetUniformLocation(program, "transform");
  checkerboardColorLocation    = glGetUniformLocation(program, "checkerboardColor");
  checkerboardContrastLocation = glGetUniformLocation(program, "checkerboardContrast");
  cursorPosLocation            = glGetUniformLocation(program, "cursorPos");
  gridSizeLocation             = glGetUniformLocation(program, "gridSize");

  const float identity[16] {
    1, 0, 0, 0,
    0, 1, 0, 0,
    0, 0, 1, 0,
    0, 0, 0, 1
  };

  setCheckerboardColor(1, 1, 1, 1);
  setCheckerboardContrast(0.9);
  setCursor(0, 0);
  setTransform(identity);

  return true;
}

void GlRenderer::blit(const float* img, std::size_t w, std::size_t h)
{
  glUniform2i(gridSizeLocation, int(w), int(h));

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_FLOAT, img);
  glGenerateMipmap(GL_TEXTURE_2D);

  glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) 0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) (sizeof(float) * 3));

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

  glDisableVertexAttribArray(1);
  glDisableVertexAttribArray(0);
}

void GlRenderer::clear(float r, float g, float b, float a)
{
  r *= a;
  g *= a;
  b *= a;

  glClearColor(r, g, b, a);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GlRenderer::setCheckerboardColor(float r, float g, float b, float a)
{
  r *= a;
  g *= a;
  b *= a;
  glUniform4f(checkerboardColorLocation, r, g, b, a);
}

void GlRenderer::setCheckerboardContrast(float contrast)
{
  glUniform1f(checkerboardContrastLocation, contrast);
}

void GlRenderer::setCursor(int x, int y)
{
  glUniform2i(cursorPosLocation, x, y);
}

void GlRenderer::setTransform(const float* data)
{
  glUniformMatrix4fv(transformLocation, 1, GL_FALSE /* no transpose */, data);
}

GLuint GlRenderer::setupShader(const char* name, const char* source, GLenum shaderType)
{
  GLuint id = glCreateShader(shaderType);
  if (!id) {
    return 0;
  }

  glShaderSource(id, 1, &source, nullptr);

  glCompileShader(id);

  GLint compileSuccess = GL_FALSE;

  glGetShaderiv(id, GL_COMPILE_STATUS, &compileSuccess);
  if (compileSuccess) {
    return id;
  }

  GLint logLength = 0;

  glGetShaderiv(id, GL_INFO_LOG_LENGTH, &logLength);

  char* msg = (char*) std::malloc(size_t(logLength));
  if (!msg) {
    return 0;
  }

  glGetShaderInfoLog(id, logLength, &logLength, msg);

  std::fprintf(stderr, "Error compiling '%s': %s", name, msg);

  std::free(msg);

  return id;
}

} // namespace px
