#version 330 core

in vec3 position;
in vec3 normal;

uniform mat4 MVP;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 lightPosition;

out vec4 normalVector;
out vec4 worldPosition;
out vec4 eyeDirection;
out vec4 lightDirection;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0f);

    worldPosition = model * vec4(position, 1.0f);

    vec4 vertexPositionCameraspace = view * model * vec4(position, 1);
    eyeDirection = -vertexPositionCameraspace;

    vec4 lightPositionCameraspace = view * vec4(lightPosition, 1.0f);
    lightDirection = lightPositionCameraspace + eyeDirection;

    normalVector = view * model * vec4(normal, 0.0f);
}