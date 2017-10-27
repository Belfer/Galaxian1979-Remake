#version 330 core

layout (location = 0) in vec4 Position;
layout (location = 1) in vec4 Color;
layout (location = 2) in vec2 TexCoord0;

out vec4 _Color;
out vec2 _TexCoord0;

uniform mat4 ModelView;
uniform mat4 Projection;

void main() {
  gl_Position = Position * ModelView * Projection;
  _Color = Color;
  _TexCoord0 = TexCoord0;
}
