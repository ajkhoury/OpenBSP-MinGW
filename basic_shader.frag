#version 330 core

uniform bool useUserColor;
uniform vec4 userColor;

out vec3 outColor;

in vec3 normalVector;
in vec3 worldPosition;
in vec3 eyeDirection;
in vec3 lightDirection;

uniform vec3 lightPosition;

void main()
{
    vec3 lightColor = vec3(1, 1, 1);
    float lightPower = 1000000.0;
    
    vec3 diffuseColor = vec3(1, 1, 1);
    vec3 ambientColor = vec3(0.1, 0.1, 0.1) * diffuseColor;
    vec3 specularColor = vec3(0.1, 0.1, 0.1);

    float distance = length(lightPosition - worldPosition);

    vec3 n = normalize( normalVector );

    // Direction of the light (from the fragment to the light)
    vec3 l = normalize( lightDirection );

    // Cosine of the angle between the normal and the light direction, clamped above 0
    //  - light is at the vertical of the triangle -> 1
    //  - light is perpendicular to the triangle -> 0
    //  - light is behind the triangle -> 0
    float cosTheta = clamp( dot(n, l), 0, 1 );

    // Eye vector (towards the camera)
    vec3 E = normalize( eyeDirection );

    // Direction in which the triangle reflects the light
    vec3 R = reflect(-l, n);

    // Cosine of the angle between the Eye vector and the Reflect vector, clamped to 0
    //  - Looking into the reflection -> 1
    //  - Looking elsewhere -> < 1
    float cosAlpha = clamp( dot(E,R), 0, 1 );

                // Ambient : simulates indirect lighting
    outColor =  ambientColor + 
                // Diffuse : "color" of the object
                diffuseColor * lightColor * lightPower * cosTheta / (distance * distance) +
                // Specular : reflective highlight, like a mirror
                specularColor * lightColor * lightPower * pow(cosAlpha, 5) / (distance * distance);

}