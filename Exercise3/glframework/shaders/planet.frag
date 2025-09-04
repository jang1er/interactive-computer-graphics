#version 330

uniform sampler2D texture1;

in vec2 fragTexcoord;

out vec4 outputColor;

void main()
{
    outputColor = vec4(texture(texture1, fragTexcoord).rgb, 1.0f);
}
