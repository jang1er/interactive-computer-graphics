#version 150

uniform float time = 0.2;

in vec2 fragComplex;

out vec4 outputColor;

vec3 hsv2rgb(vec3 c)
{
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

vec2 multiply_complex(vec2 c0, vec2 c1) {
    return vec2(c0.x*c1.x - c0.y*c1.y, c0.y*c1.x + c0.x*c1.y);
}

vec3 color_lerp(vec3 color1, vec3 color2, float ratio){
    return color1 * (1 - ratio) + color2 * ratio;
}

void main()
{
    const int maxIteration = 128;
    const float bailout = 1e20;
    const vec2 cj0 = vec2(-0.835, -0.3842);
    const vec2 cj1 = vec2(-0.65176, 0.256);
    const vec3 color0 = vec3(0, 1, 1);
    const vec3 color1 = vec3(1, 1, 1);

    vec3 color = vec3(0.0);

    float tx = 0.5 * (cos(time) + 1.0);
    float ty = 0.5 * (sin(time * 0.777) + 1.0);

    vec2 z = fragComplex;
    vec2 c = vec2(mix(cj0.x, cj1.x, tx), mix(cj0.y, cj1.y, ty));
    color = vec3(fragComplex,0);

    // Question 2c
    int iterations = 0;
    while(dot(z,z) <= bailout && iterations <= maxIteration){
        z = multiply_complex(z,z) + c;
        ++iterations;
    }
    if(iterations > maxIteration){
        // did not diverge
        color = vec3(0); // black
    } else {
        // did diverge
        // define color on steps left to maximum
        color = hsv2rgb(color_lerp(color0, color1, float(iterations) / float(maxIteration)));
        //color = vec3(float(iterations) / float(maxIteration));
    }
    outputColor = vec4(color,1);
}
