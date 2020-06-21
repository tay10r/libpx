#include "Shaders.hpp"

namespace px {

const char* vertexShader = R"(
#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 texCoord;

uniform mat4 transform = mat4(1.0);

out vec2 texCoord2;

void main() {
  gl_Position = transform * vec4(pos, 1);
  texCoord2 = texCoord;
}
)";

const char* fragmentShader = R"(
#version 330 core

out vec4 color;

in vec2 texCoord2;

in vec4 gl_FragCoord;

uniform sampler2D imageTexture;

uniform ivec2 cursorPos = ivec2(0, 0);

uniform ivec2 gridSize = ivec2(4, 4);

uniform float checkerboardContrast = 0.1;

uniform vec4 checkerboardColor = vec4(1.0, 1.0, 1.0, 1.0);

void main() {

  vec2 pos = floor(texCoord2 * gridSize);

  float hoverMask = ((int(pos.x) == cursorPos.x)
                  && (int(pos.y) == cursorPos.y)) ? 1 : 0;

  float patternMask = mod(pos.x + mod(pos.y, 2.0), 2.0);

  patternMask = (1.0 - checkerboardContrast)
              + (patternMask * checkerboardContrast)
              + (0.5 - (0.5 * hoverMask));

  patternMask /= 1.5;

  vec4 bg = patternMask * checkerboardColor;

  /* Maintain the original alpha value. */
  bg.a = checkerboardColor.a;

  vec4 fg = texture(imageTexture, texCoord2);

  color = fg + (bg * (1.0 - fg.a));
}
)";

namespace webgl {

const char* vertexShader = R"(
#version 100

attribute vec4 a_position;
attribute vec2 a_texCoord;

uniform mat4 transform;

varying vec2 v_texCoord;

void main()
{
  gl_Position = transform * a_position;

  v_texCoord = a_texCoord;
}
)";

const char* fragmentShader = R"(
#version 100

precision mediump float;

varying vec2 v_texCoord;

uniform sampler2D s_texture;

uniform ivec2 cursorPos;

uniform ivec2 gridSize;

uniform float checkerboardContrast;

uniform vec4 checkerboardColor;

void main()
{
  float contrast = 0.9;

  vec4 color = vec4(0.5, 0.5, 0.5, 1.0);

  vec2 pos = floor(v_texCoord * vec2(gridSize.x, gridSize.y));

  float hoverMask = ((int(pos.x) == cursorPos.x)
                  && (int(pos.y) == cursorPos.y)) ? 1.0 : 0.0;

  float patternMask = mod(pos.x + mod(pos.y, 2.0), 2.0);

  patternMask = (1.0 - checkerboardContrast)
              + (patternMask * checkerboardContrast)
              + (0.5 - (0.5 * hoverMask));

  patternMask /= 1.5;

  vec4 bg = patternMask * checkerboardColor;

  /* Maintain the original alpha value. */
  bg.a = checkerboardColor.a;

  vec4 fg = texture2D(s_texture, v_texCoord);

  gl_FragColor = fg + (bg * (1.0 - fg.a));
}
)";
} // namespace webgl

} // namespace px
