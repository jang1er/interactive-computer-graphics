#version 150

in vec4 vertexPosition;
in vec3 vertexColor;

uniform mat4 MVP;

out vec3 fragColor;

void main()
{
    fragColor = vertexColor;
    gl_Position = MVP * vertexPosition;
}
