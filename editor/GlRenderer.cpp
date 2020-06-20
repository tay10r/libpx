#include "GlRenderer.hpp"

#include "Shaders.hpp"

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

#ifndef __EMSCRIPTEN__
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
#endif

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

#ifdef __EMSCRIPTEN__
  const char* vShaderSource = px::webgl::vertexShader;
  const char* fShaderSource = px::webgl::fragmentShader;
#else
  const char* vShaderSource = px::vertexShader;
  const char* fShaderSource = px::fragmentShader;
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

  transformLocation         = glGetUniformLocation(program, "transform");
  checkerboardColorLocation = glGetUniformLocation(program, "checkerboardColor");
  cursorPosLocation         = glGetUniformLocation(program, "cursorPos");

  return true;
}

void GlRenderer::blit(const float* img, std::size_t w, std::size_t h)
{
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
  glClearColor(r, g, b, a);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GlRenderer::setCheckerboardColor(float, float, float, float)
{
}

void GlRenderer::setCheckerboardContrast(float)
{
}

void GlRenderer::setCursor(int, int)
{
}

void GlRenderer::setTranslation(float, float)
{
}

void GlRenderer::setZoom(float)
{
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
