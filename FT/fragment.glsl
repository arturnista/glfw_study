#version 130

out vec4 fragment_color;
in vec3 ourColor;
void main() {
    fragment_color = vec4(ourColor, 1.0);
}
