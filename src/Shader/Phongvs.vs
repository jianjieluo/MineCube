#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec4 colorInstance;
layout (location = 3) in mat4 modleInstanceMatrix;

out vec3 FragPos;
out vec3 Normal;
out vec4 OurColor;
out vec4 FragPosLightSpace;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightSpaceMatrix;

void main()
{
    FragPos = vec3(modleInstanceMatrix * vec4(aPos, 1.0));
    OurColor = colorInstance;
    Normal = mat3(transpose(inverse(modleInstanceMatrix))) * aNormal;  
    FragPosLightSpace = lightSpaceMatrix * vec4(FragPos, 1.0);

    gl_Position = projection * view * vec4(FragPos, 1.0);
}