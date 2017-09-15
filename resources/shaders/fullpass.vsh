#version 330

in vec4 Position;

out vec2 vUV;

void main()
{
    vUV = (Position.xy + vec2(1, 1)) * 0.5;
	gl_Position = Position;
}
