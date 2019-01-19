#version 330 core

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec2 aTexCoord;

out vec4 posPass;
out vec2 TexCoord;

uniform mat4 model;
uniform mat4 proj;
uniform mat4 view;

void main()
{
    posPass = proj *view * model * vec4(inPos, 1.0f);
    gl_Position = posPass;
    TexCoord = vec2(aTexCoord.x, aTexCoord.y);
}
