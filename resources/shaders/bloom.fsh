#version 330

in vec2 vUV;
out vec4 outColour;

uniform sampler2D renderTarget;
uniform float time;
uniform vec4 color;
uniform vec4 position;
uniform vec4 offset;

const float blurSizeH = 1.0 / 300.0;
const float blurSizeV = 1.0 / 400.0;
const int it = 4;
const float k = 81.0;

void main()
{
    vec4 sum = vec4(0);
    for (int x = -it; x <= it; x++)
        for (int y = -it; y <= it; y++)
            sum += texture(renderTarget, vec2(vUV.x + x * blurSizeH, vUV.y + y * blurSizeV)) / k;

    vec4 col = sum + texture2D(renderTarget, vUV);
    outColour = col * color;
}
