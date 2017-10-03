#version 130

out vec4 fragment_color;

in vec3 ourColor;
in vec3 lColor;

void main() {
    float ambientStrength = .1;
    vec3 ambient = ambientStrength * lColor;

    vec3 result = ambient * ourColor;
    fragment_color = vec4(result, 1.0);
}
