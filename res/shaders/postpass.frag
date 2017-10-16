#version 330

in vec2 vUV;
out vec4 outColour;

uniform sampler2D renderTarget;
uniform vec2 resolution;
uniform float time;
uniform float bloomK;
uniform vec4 waveParams = vec4(10.0, 0.6, 0.02, 1.0);
uniform vec4 color;
uniform vec4 position;
uniform vec4 offset;

const int it = 4;

void main()
{
    // Calculate shock wave UVs
    vec2 texCoord = vUV;
    vec2 center = position.xy;
    vec2 dir = vUV - center;
    
    float dist = length(dir);
    if ((dist <= (time + waveParams.z)) &&
        (dist >= (time - waveParams.z)) ) {
        float diff = (dist - time) * waveParams.w;
        float diff3 = pow(diff, 3);
        
        float offset = diff3 * (1.0 - pow(abs(diff3 * waveParams.x), waveParams.y));
        dir = normalize(dir);
        
        texCoord = vUV + (dir * offset);
    }
    
    // Chromatic aberration
    vec4 r = texture2D(renderTarget, texCoord - sin(time+1));  
    vec4 g = texture2D(renderTarget, texCoord - cos(time));
    vec4 b = texture2D(renderTarget, texCoord - cos(time+1));  

    vec4 chrab = vec4(r.r, g.g, b.b, 1.0);

    // Bloom pass
    vec4 sum = vec4(0);
    for (int x = -it; x <= it; x++)
        for (int y = -it; y <= it; y++)
            sum += texture2D(renderTarget,
                           vec2(texCoord.x + x * resolution.x,
                                texCoord.y + y * resolution.y)) / bloomK;
    vec4 bloom = sum + texture2D(renderTarget, texCoord);
    
    outColour = bloom * color;
}
