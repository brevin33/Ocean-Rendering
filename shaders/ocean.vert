#version 450

// Camera
layout(binding = 0) uniform cameraInfo{ 
    mat4 viewproj;
} camera;

layout(binding = 0) uniform modelInfo{
    mat4 inModel;
}model;

// Vertex Data
layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;


// Frag Shader Data
layout(location = 0) out vec3 fragColor;

void main() {
    gl_Position = camera.viewproj * mat4(1) * vec4(inPosition, 1.0);

    fragColor = inColor;
}