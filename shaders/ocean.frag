#version 450

layout (location = 0) in float Height;

layout (location = 0) out vec4 FragColor;

void main()
{
    vec3 lightDir = vec3(0.1,-1, -0.1);
    lightDir = normalize(lightDir);
    vec3 color1 = vec3(9.0/255.0,158.0/255.0,210.0/255.0);
    vec3 color2 = vec3(124.0/255.0,248.0/255.0,253.0/255.0);
    float h = (Height+13.0f)/26.0f;
    FragColor = vec4(mix(color1, color2, h), 1.0);
}