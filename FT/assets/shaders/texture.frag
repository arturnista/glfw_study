#version 130

out vec4 fragment_color;

in vec3 Normal;
in vec3 FragPos;
in vec2 TextureCoord;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPosition;

uniform vec3 viewPos;
uniform mat4 view;
uniform mat4 model;
uniform mat4 inverseModel;
uniform mat4 projection;
uniform mat4 transform;

uniform sampler2D ourTexture;

void main() {
    float ambientStrength = .1;
    vec3 ambient = ambientStrength * lightColor;

    vec3 normalNormalized = normalize(Normal);
    vec3 lightDirection = normalize(lightPosition - FragPos);

    float diff = max(dot(normalNormalized, lightDirection), 0.0);
    vec3 diffuse = diff * lightColor;

    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDirection, normalNormalized);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 128);
    vec3 specular = specularStrength * spec * lightColor;

    vec3 result = (ambient + diffuse + specular) * objectColor;
    fragment_color = texture(ourTexture, TextureCoord) * vec4(result, 1.0);
}
