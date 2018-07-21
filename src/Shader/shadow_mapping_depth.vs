#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec4 colorInstance;
layout (location = 3) in mat4 modleInstanceMatrix;

uniform mat4 lightSpaceMatrix;

void main()
{
    gl_Position = lightSpaceMatrix * modleInstanceMatrix * vec4(aPos, 1.0);
}