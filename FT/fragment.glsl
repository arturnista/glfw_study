#version 130

out vec4 fragment_color;

in vec3 Color;
in vec3 LColor;
in vec3 Normal;
in vec3 FragPos;
in vec3 LPosition;
in vec3 ViewPos;

void main() {
    float ambientStrength = .1;
    vec3 ambient = ambientStrength * LColor;

    vec3 normalNormalized = normalize(Normal);
    vec3 lightDirection = normalize(LPosition - FragPos);

    float diff = max(dot(normalNormalized, lightDirection), 0.0);
    vec3 diffuse = diff * LColor;

    float specularStrength = 0.5;
    vec3 viewDir = normalize(ViewPos - FragPos);
    vec3 reflectDir = reflect(-lightDirection, normalNormalized);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 128);
    vec3 specular = specularStrength * spec * LColor;

    vec3 result = (ambient + diffuse + specular) * Color;
    fragment_color = vec4(result, 1.0);
}
