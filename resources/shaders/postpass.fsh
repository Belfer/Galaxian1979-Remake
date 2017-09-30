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

const vec3 shockParams = vec3(10.0, 0.6, 0.02);

void main()
{
    // Calculate shock wave UVs
    vec2 texCoord = vUV;
    vec2 center = position.xy;
    float distance = distance(vUV, center);
    if ((distance <= (time + shockParams.z)) &&
        (distance >= (time - shockParams.z))) {
        
        float diff = (distance - time);
        float powDiff = 1.0 - pow(abs(diff * shockParams.x), shockParams.y);
        float diffTime = diff  * powDiff;
        vec2 diffUV = normalize(vUV - center);
        texCoord = vUV + (diffUV * diffTime);
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
            sum += texture(renderTarget, vec2(texCoord.x + x * blurSizeH, texCoord.y + y * blurSizeV)) / k;
    vec4 bloom = sum + texture2D(renderTarget, texCoord);
    
    outColour = bloom * color;
}
