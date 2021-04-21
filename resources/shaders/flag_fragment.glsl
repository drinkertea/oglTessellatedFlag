#version 330 core
in vec2  texCoord;
in float amplitudeVal; // from 0 to 1 ranged depth value

uniform sampler2D texture1;

out vec4 fragColor;

void main()
{
    float resolve = abs(float(!gl_FrontFacing) - amplitudeVal);
    resolve = 0.4 + 0.6 * resolve;
    fragColor = resolve * texture(texture1, texCoord);
}
