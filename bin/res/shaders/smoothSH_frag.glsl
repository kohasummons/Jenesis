

#version 330 core


in vec4 color;
in vec3 position;
in vec3 normal;

out vec4 FragColor;

uniform vec3 lightColor;
uniform vec3 lightPosition;
uniform float ambience;
uniform vec3 camPos;


void main()
{
    vec3 Normal = normalize(normal);
    
    vec3 lightDirection = normalize(lightPosition - position);

    float diffuse = max(dot(Normal, lightDirection), 0.0f);

    float specularLight = 0.5f;
    vec3 viewDirection = normalize(camPos - position);
    vec3 reflectionDirection = reflect(-lightDirection, Normal);
    float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 15);
    float specular = specAmount - specularLight;

    FragColor = vec4(color) * vec4(lightColor, 1.0f) * (diffuse + ambience + specular);
}