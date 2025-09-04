#version 150

in vec3 fragColor;

out vec4 outputColor;

void main()
{
    outputColor.rgb = fragColor;
}
