#version 330 core

in vec4 _Color;
in vec2 _TexCoord0;

out vec4 _FragColor;

uniform sampler2D Diffuse;

void main() {
  _FragColor = _Color * texture2D(Diffuse, _TexCoord0);
}
