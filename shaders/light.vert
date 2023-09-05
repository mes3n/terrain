#version 330 core
layout (location = 0) in vec3 vertexPos;
layout (location = 1) in vec3 vertexNormal;

uniform mat4 matrix3d;
uniform vec3 lightOrigin = vec3(50.0, 150.0, 50.0);

void main() 
{
    gl_Position = matrix3d * vec4(vertexPos + lightOrigin, 1.0);
}
