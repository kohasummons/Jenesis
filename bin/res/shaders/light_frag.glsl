

#version 330 core


in vec4 color;

uniform vec4 lightColor;


void main()
{
    FragColor = vec4(lightColor);
}