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
    if (!useUserColor)
    {
        // For individual lights
        //vec3 lightColor = vec3(1, 1, 1);
        float lightPower = 3.0f;

        vec3 diffuse = vec3(1, 1, 1);
        vec3 ambient = vec3(0.1, 0.1, 0.1) * diffuse;
        vec3 specular = vec3(0.4, 0.4, 0.4);

        vec3 n = normalize( normalVector );
        vec3 l = normalize( lightDirection );
        vec3 e = normalize( eyeDirection );

        vec3 spec = vec3(0.0);

        float intensity = max(dot(n, l), 0.0);

        // if the vertex is lit compute the specular color
        if (intensity > 0.0) 
        {
            // compute the half vector
            vec3 h = normalize(l + e);  
            // compute the specular term into spec
            float intSpec = max(dot(h, n), 0.0);
            spec = specular * pow(intSpec, 128);
        }

        outColor =  max(intensity *  diffuse + spec, ambient);
    }
    else
    {
        outColor = userColor.xyz;
        
    }

}