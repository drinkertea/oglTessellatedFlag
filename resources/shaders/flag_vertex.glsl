#version 330 core
#define M_PI 3.1415926535897932384626433832795

layout (location = 0) in vec3 aPos;

uniform mat4  viewProj;
uniform mat4  model;
uniform float time;
uniform float amplitude;
uniform float waveCount;

out vec2  plotCoord;
out float amplitudeVal;

void main()
{
    vec3 pos = aPos;
    float coef = M_PI * 2.0 * waveCount;

    // Simulate waves with sin. Start point must be fixed.
    pos.z = amplitude * (sin(time * coef) + sin((pos.x - time)*coef));
    pos.z = pos.z * 0.5;

    gl_Position  = viewProj * model* vec4(pos, 1.0);
    plotCoord    = vec2(pos.x, pos.y);

    // Forward apliude to FS to draw shadows
    amplitudeVal = (pos.z + amplitude) / (2.0 * amplitude);
}
