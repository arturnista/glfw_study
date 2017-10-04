#version 130
#extension GL_ARB_explicit_attrib_location : enable

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec3 aNormal;

out vec3 Color;
out vec3 Normal;
out vec3 LColor;
out vec3 LPosition;
out vec3 FragPos;
out vec3 ViewPos;

uniform vec3 lightColor;
uniform vec3 lightPosition;

uniform vec3 viewPos;
uniform mat4 view;
uniform mat4 model;
uniform mat4 inverseModel;
uniform mat4 projection;
uniform mat4 transform;

void main() {
    gl_Position = projection * view * model * transform * vec4(aPos, 1.0);
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverseModel)) * aNormal;
    Color = aColor;
    LColor = lightColor;
    LPosition = lightPosition;
    ViewPos = viewPos;
}
