#shader vertex
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec2 TexCoord;
out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    TexCoord = aTexCoord;
    Normal = mat3(transpose(inverse(model))) * aNormal;
}

#shader fragment
#version 330 core

out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

void main()
{
    vec4 diffuseColor = texture(texture_diffuse1, TexCoord);
    vec4 specularColor = texture(texture_specular1, TexCoord);
    
    FragColor = diffuseColor + specularColor * 0.5;
}