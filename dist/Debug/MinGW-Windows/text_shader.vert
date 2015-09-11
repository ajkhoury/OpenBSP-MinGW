#version 330 core

in vec4 in_Position;
out vec2 texCoords;

void main(void)
{
    gl_Position = vec4(in_Position.xy, 0, 1);
    texCoords = in_Position.zw;
}