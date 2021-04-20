#version 330 core
#define M_PI 3.1415926535897932384626433832795

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

uniform mat4 viewProj;
uniform float time;
uniform float amplitude;
uniform float waveCount;
uniform float xOffset;

void main()
{
    vec3 pos = aPos;
    float coef = M_PI * 2.0 * waveCount;
    pos.z += amplitude * (sin(time * coef) + sin((pos.x - time - xOffset)*coef));

    gl_Position = viewProj * vec4(pos, 1.0);
    TexCoord = vec2(aTexCoord.x, aTexCoord.y);
}
