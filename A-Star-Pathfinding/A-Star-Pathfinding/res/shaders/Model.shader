#shader vertex
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

layout (location = 3) in ivec4 aBoneIDs;
layout (location = 4) in vec4  aWeights;

out vec2 TexCoord;
out vec3 Normal;

flat out ivec4 boneIDs;
out vec4       weights;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

const int MAX_BONES = 100;
uniform mat4 bones[MAX_BONES];

void main()
{
    TexCoord = aTexCoord;
    boneIDs  = aBoneIDs;
    weights  = aWeights;

    mat4 boneTransform = bones[BoneIDS[0]] * weights[0];
    boneTransform     += bones[BoneIDS[1]] * weights[1];
    boneTransform     += bones[BoneIDS[2]] * weights[2];
    boneTransform     += bones[BoneIDS[3]] * weights[3];

    vec4 localSpace = boneTransform * vec4(aPos, 1.0);

    gl_Position = projection * view * model * localSpace;

    Normal = mat3(transpose(inverse(model))) * aNormal;
}

#shader fragment
#version 330 core

out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
flat in ivec4 boneIDs;
in vec4       weights;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform int displayBoneIndex;

void main()
{
    vec4 diffuseColor = texture(texture_diffuse1, TexCoord);
    vec4 specularColor = texture(texture_specular1, TexCoord);
    
    bool found = false;

    for(int i = 0; i < 4; i++)
    {
        if(boneIDs[i] == displayBoneIndex)
        {
            if(weights[i] >= 0.7)
            {
                FragColor = vec4(1.0, 0.0, 0.0, 1.0) * weights[i];
            }
            else if(weights[i] >= 0.4 && weights[i] <= 0.6)
            {
                FragColor = vec4(0.0, 1.0, 0.0, 1.0) * weights[i];
            }
            else if(weights[i] >= 0.1)
            {
                FragColor = vec4(1.0, 1.0, 0.0, 1.0) * weights[i];
            }

            found = true;
            break;
        }
    }

    if(!found)
    {
        FragColor = vec4(0.0, 0.0, 1.0, 1.0);
    }


    // FragColor = diffuseColor + specularColor * 0.5;
}