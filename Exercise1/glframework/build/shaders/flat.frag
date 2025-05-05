#version 150

// Interpolated values from the vertex shaders
in vec3 fragmentColor;

out vec4 Color;

void main() {
    Color = vec4(fragmentColor.rgb, 1.0);
}
