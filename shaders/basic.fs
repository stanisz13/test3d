#version 330 core

in vec4 posPass;
in vec2 TexCoord;

out vec4 fragColor;

uniform sampler2D tex;

void main()
{
    fragColor = texture(tex, TexCoord);
}
