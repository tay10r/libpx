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

uniform float contrast = 0.1;

uniform ivec2 cursorPos = ivec2(0, 0);

uniform vec3 checkerboardColor = vec3(1.0, 1.0, 1.0);

void main() {

  ivec2 texelSize = textureSize(imageTexture, 0);

  vec2 pos = floor(texCoord2 * texelSize);

  float hoverMask = ((int(pos.x) == cursorPos.x)
                  && (int(pos.y) == cursorPos.y)) ? 1 : 0;

  float patternMask = mod(pos.x + mod(pos.y, 2.0), 2.0);

  patternMask = (1.0 - contrast) + (patternMask * contrast) + (0.5 - (0.5 * hoverMask));

  patternMask /= 1.5;

  vec4 bg = vec4(patternMask * vec3(1.0, 1.0, 1.0), 1.0);

  vec4 fg = texture(imageTexture, texCoord2);

  color = fg + (bg * (1.0 - fg.a));
}
)";

namespace webgl {

const char* vertexShader = R"(
#version 100

attribute vec4 a_position;
attribute vec2 a_texCoord;

varying vec2 v_texCoord;

void main()
{
  gl_Position = a_position;

  v_texCoord = a_texCoord;
}
)";

const char* fragmentShader = R"(
#version 100

precision mediump float;

varying vec2 v_texCoord;

uniform sampler2D s_texture;

void main()
{
  float contrast = 0.9;

  vec4 color = vec4(0.5, 0.5, 0.5, 1.0);

  vec2 texelSize = vec2(4, 4); /* textureSize(imageTexture, 0); */

  vec2 pos = floor(v_texCoord * texelSize);

  ivec2 cursorPos = ivec2(0, 0);

  float hoverMask = ((int(pos.x) == cursorPos.x)
                  && (int(pos.y) == cursorPos.y)) ? 1.0 : 0.0;

  float patternMask = mod(pos.x + mod(pos.y, 2.0), 2.0);

  patternMask = (1.0 - contrast) + (patternMask * contrast) + (0.5 - (0.5 * hoverMask));

  patternMask /= 1.5;

  vec4 bg = vec4(patternMask * vec3(1.0, 1.0, 1.0), 1.0);

  /* vec4 fg = texture2D(s_texture, v_texCoord); */

  vec4 fg = vec4(0, 0, 0, 0.2);

  gl_FragColor = fg + (bg * (1.0 - fg.a));
}
)";
} // namespace webgl

} // namespace px
