#version 330

in vec4 vertexPosition;
in vec2 vertexTexcoord;

uniform mat4 MVP;

out vec2 fragTexcoord;

void main()
{
    fragTexcoord = vertexTexcoord;
    gl_Position = MVP * vertexPosition;
}
