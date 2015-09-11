#version 330 core

uniform sampler2D tex;
uniform vec4 color;

in vec2 texCoords;
out vec4 fragColor;

void main(void)
{
    fragColor = vec4(1, 1, 1, texture(tex, texCoords).r) * color;
}