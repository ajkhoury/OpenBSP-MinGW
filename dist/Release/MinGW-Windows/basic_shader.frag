#version 330 core

uniform bool useUserColor;
uniform vec4 userColor;

out vec4 outColor;
in vec3 normalColor;

void main()
{
    if (useUserColor)
    {
        outColor = userColor;
    }
    else
    {
        outColor = vec4(normalColor.x, normalColor.y, normalColor.z, 1.0);
    }
}