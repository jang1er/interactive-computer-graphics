#version 150

uniform mat4 MVP;

// Input vertex data, different for all executions of this shader.
in vec3 vertexPosition;
in vec2 vertexTexcoord;
in vec3 vertexNormal;
in vec3 vertexColor;

// Output data, will be interpolated for each fragment.
out vec2 fragmentTexcoord;
out vec3 fragmentNormal;
out vec3 fragmentColor;

void main() {
    gl_Position = MVP * vec4(vertexPosition, 1.0);
	fragmentTexcoord = vertexTexcoord;
	fragmentNormal = vertexNormal;
	fragmentColor = vertexColor;
}
