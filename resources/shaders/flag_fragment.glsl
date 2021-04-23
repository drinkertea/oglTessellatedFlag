#version 330 core
#define M_PI 3.1415926535897932384626433832795

in vec2  plotCoord;    // 2d coordinates on flag plot.
in float amplitudeVal; // from 0 to 1 ranged depth value

out vec4 fragColor;

bool SameSide(vec2 p1, vec2 p2, vec2 a, vec2 b)
{
    vec3 cp1 = cross(vec3(b - a, 0), vec3(p1 - a, 0));
    vec3 cp2 = cross(vec3(b - a, 0), vec3(p2 - a, 0));
    return dot(cp1, cp2) >= 0;
}

bool PointInTriangle(vec2 p, vec2 a, vec2 b, vec2 c)
{
    return SameSide(p,a, b,c) && SameSide(p,b, a,c) && SameSide(p,c, a,b);
}

bool DrawStar(vec2 coord, float smallRad, float bigRad)
{
    // Define star angles
    float angles[10];
    for (int i = 0; i < 10; ++i)
        angles[i] = float(i) * (-M_PI) / 5;

    vec2 starPoints[10];
    for (int i = 0; i < 10; ++i)
    {
        // Inner and outer vertices of the star.
        float scale = i % 2 == 0 ? smallRad : bigRad;
        starPoints[i] = scale * vec2(cos(angles[i]), sin(angles[i]));
    }
    // Star geometry by 8 triangles.
    return PointInTriangle(coord, starPoints[9], starPoints[0], starPoints[1]) ||
           PointInTriangle(coord, starPoints[1], starPoints[2], starPoints[3]) ||
           PointInTriangle(coord, starPoints[3], starPoints[4], starPoints[5]) ||
           PointInTriangle(coord, starPoints[5], starPoints[6], starPoints[7]) ||
           PointInTriangle(coord, starPoints[7], starPoints[8], starPoints[9]) ||
           PointInTriangle(coord, starPoints[1], starPoints[3], starPoints[5]) ||
           PointInTriangle(coord, starPoints[1], starPoints[5], starPoints[7]) ||
           PointInTriangle(coord, starPoints[1], starPoints[7], starPoints[9]);

}

float DrawCircle(vec2 coord, float radius) {
    return step(length(coord), radius);
}

void main()
{
    vec2  coord       = plotCoord;

    // Moon by two circles.
    float bigCircle   = DrawCircle(coord - vec2(0.75, 0.5), 0.25);
    float smallCircle = DrawCircle(coord - vec2(0.81, 0.5), 0.2);
    float star        = float(DrawStar(coord - vec2(0.85, 0.5), 0.125, 0.0475));

    // Result for red color. Moon or star.
    float redRes      = clamp(bigCircle - smallCircle + star, 0.0, 1.0);

    // Green/White split.
    float left        = step(coord.x, 0.75) - redRes;

    // Result color. Red or green or white.
    vec3  color       = redRes * vec3(0.82, 0.06, 0.2) + (1.0 - redRes) *
                         (left * vec3(0.0,  0.4,  0.2) + (1.0 - left) * vec3(1.0));

    // Simulate hight-based shadow.
    float resolve     = 0.3 + 0.7 * (abs(float(!gl_FrontFacing) - amplitudeVal));

    fragColor         = resolve * vec4(color, 1.0);
}
