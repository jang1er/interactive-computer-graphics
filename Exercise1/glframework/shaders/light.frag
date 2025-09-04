#version 150

// Interpolated values from the vertex shaders
in vec3 fragmentNormal;
in vec3 fragmentColor;

out vec4 Color;

void main() {
    const vec3 lightDir = normalize(vec3(0.2f, 1.0f, 0.4f));
    float cosang = clamp(dot(normalize(fragmentNormal), lightDir), 0.0f, 1.0f);
    Color = vec4(fragmentColor * (cosang * 0.8 + 0.2), 1.0);
    //Color = vec4((fragmentNormal + 1) / 2, 1.0);
    //Color = vec4(cosang * 0.8 + 0.2, 0.0, 0.0, 1.0);
}
