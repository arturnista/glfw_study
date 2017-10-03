#version 130
#extension GL_ARB_explicit_attrib_location : enable

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 ourColor;
out vec3 lColor;

uniform vec3 lightColor;

uniform mat4 view;
uniform mat4 model;
uniform mat4 projection;
uniform mat4 transform;

void main() {
    gl_Position = projection * view * model * transform * vec4(aPos, 1.0);
    ourColor = aColor;
    lColor = lightColor;
}
