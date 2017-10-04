#version 130

out vec4 fragment_color;

in vec3 Color;
in vec3 LColor;
in vec3 Normal;
in vec3 FragPos;
in vec3 LPosition;

void main() {
    float ambientStrength = .1;
    vec3 ambient = ambientStrength * LColor;

    vec3 normalNormalized = normalize(Normal);
    vec3 lightDirection = normalize(LPosition - FragPos);

    float diff = max(dot(normalNormalized, lightDirection), 0.0);
    vec3 diffuse = diff * LColor;

    vec3 result = (ambient + diffuse) * Color;
    fragment_color = vec4(result, 1.0);
}
