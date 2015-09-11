#version 330 core

in vec4 coord;
out vec2 texCoords;

void main(void)
{
    gl_Position = vec4(coord.xy, 0, 1);
    texCoords = coord.zw;
}