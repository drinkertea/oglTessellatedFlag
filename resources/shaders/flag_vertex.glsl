#version 330 core
#define M_PI 3.1415926535897932384626433832795

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

uniform mat4  viewProj;
uniform mat4  model;
uniform float time;
uniform float amplitude;
uniform float waveCount;

out vec2  texCoord;
out float amplitudeVal;

void main()
{
    vec3 pos = aPos;
    float coef = M_PI * 2.0 * waveCount;
    pos.z += amplitude * (sin(time * coef) + sin((pos.x - time)*coef));

    gl_Position = viewProj * model* vec4(pos, 1.0);
    texCoord = vec2(aTexCoord.x, aTexCoord.y);
    amplitudeVal = (pos.z + 2.0 * amplitude) / (4.0 * amplitude);
}
