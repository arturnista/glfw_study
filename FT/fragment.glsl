#version 150

out vec4 frag_colour;

uniform vec4 ourColor; // we set this variable in the OpenGL code.
void main() {
    frag_colour = ourColor;
    // frag_colour = vec4(0.5, 0.0, 0.5, 1.0);
};
