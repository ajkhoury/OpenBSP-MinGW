#version 330 core

in vec3 position;
in vec3 normal;

uniform mat4 MVP;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 lightPosition;

out vec3 normalVector;
out vec3 worldPosition;
out vec3 eyeDirection;
out vec3 lightDirection;

void main()
{
    gl_Position = MVP * vec4(position, 1);

    worldPosition = (model * vec4(position, 1)).xyz;

    vec3 vertexPositionCameraspace = ( view * model * vec4(position, 1)).xyz;
    eyeDirection = vec3(0,0,0) - vertexPositionCameraspace;

    vec3 lightPositionCameraspace = ( view * vec4(lightPosition, 1)).xyz;
    lightDirection = lightPositionCameraspace + eyeDirection;

    normalVector = (view * model * vec4(normal, 0)).xyz;
    
}