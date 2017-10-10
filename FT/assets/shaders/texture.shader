#version 130
#extension GL_ARB_explicit_attrib_location : enable

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTextureCoord;

out vec3 Color;
out vec3 Normal;
out vec3 FragPos;
out vec2 TextureCoord;

uniform mat4 view;
uniform mat4 model;
uniform mat4 inverseModel;
uniform mat4 projection;

void main() {
    TextureCoord = aTextureCoord;

    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverseModel)) * aNormal;
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
