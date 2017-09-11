#version 150
#extension GL_ARB_explicit_attrib_location : enable

layout (location = 0) in vec3 aPos;
void main() {
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
};
