#version 460 core
layout (location = 0) in vec3 vertexPos;
layout (location = 1) in vec3 vertexNormal;

out vec3 fragNormal;
out vec3 fragPos;

uniform mat4 matrix3d;

void main () 
{
    gl_Position = matrix3d * vec4(vertexPos, 1.0);
    fragNormal = vertexNormal;
    fragPos = vertexPos;
}
