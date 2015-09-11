#version 330 core

in vec3 position;
in vec3 nColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 normalColor;

void main()
{
    normalColor = nColor;
    gl_Position = projection * view * model * vec4 (position, 1.0);
}