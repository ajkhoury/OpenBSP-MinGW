#version 330 core

uniform bool useUserColor;
uniform vec4 userColor;

uniform vec3 lightPosition;

out vec4 outColor;

in vec4 normalVector;
in vec4 worldPosition;
in vec4 eyeDirection;
in vec4 lightDirection;

void main()
{
    if (useUserColor)
    {
        outColor = userColor;
    } 
    else
    {
        vec4 diffuse = vec4(1.0f, 1.0f, 1.0f, 1.0f);
        vec4 ambient = vec4(0.1f, 0.1f, 0.1f, 1.0f) * diffuse;
        vec4 specular = vec4(0.4f, 0.4f, 0.4f, 1.0f);

        //vec4 final_color = diffuse * ambient

        vec4 n = normalize( normalVector );
        vec4 l = normalize( lightDirection );
        vec4 e = normalize( eyeDirection );

        vec4 spec = vec4(0.0);

        float intensity = max(dot(n, l), 0.0);

        // if the vertex is lit compute the specular color
        if (intensity > 0.0) 
        {
            // compute the half vector
            vec4 h = normalize(l + e);  
            // compute the specular term into spec
            float intSpec = max(dot(h, n), 0.0);
            spec = specular * pow(intSpec, 128);
        }

        outColor =  max(intensity *  diffuse + spec, ambient);
    }
}