#include "Animator.h"
#include "AssimpUtilities.h"
#include <iostream>

Animator::Animator(Model* model)
    : currentModel(model), currentTime(0.0f), currentAnimation(nullptr)
{
    finalBoneMatrices.assign(100, glm::mat4(1.0f));
}

void Animator::PlayAnimation(Animation* animation)
{
    currentAnimation = animation;
    currentTime      = 0.0f;
}

void Animator::UpdateAnimation(float deltaTime)
{
    if (!currentAnimation) 
        return;

    float ticksPerSecond = currentAnimation->GetTicksPerSecond();
    currentTime         += (ticksPerSecond != 0 ? ticksPerSecond : 25.0f) * deltaTime;
    currentTime          = fmod(currentTime, currentAnimation->GetDuration());

    CalculateBoneTransform(&currentAnimation->GetRootNode(), glm::mat4(1.0f));
}

void Animator::CalculateBoneTransform(const AssimpNodeData* node, glm::mat4 parentTransform)
{
    std::string originalName = node->name;
    std::string cleanName    = node->name;

    //MIXAMO support
    if (cleanName.find("mixamorig:") != std::string::npos)
        cleanName = cleanName.substr(10);

    glm::mat4 nodeTransform = node->transformation;

    //FBX support
    if (originalName.find("$AssimpFbx$") != std::string::npos)
        nodeTransform = glm::mat4(1.0f);

    Bone* bone = currentAnimation->FindBone(cleanName);
    if (bone)
    {
        bone->Update(currentTime);
        nodeTransform = bone->GetLocalTransform();
    }

    glm::mat4 globalTransform = parentTransform * nodeTransform;

    auto& boneInfoMap = currentModel->GetBoneNameToInfo();

    //Check oryginal and clean name, model can have bone with prefix or without it
    if (boneInfoMap.count(originalName))
    {
        const auto& info = boneInfoMap.at(originalName);
        finalBoneMatrices[info.id] = currentModel->GetGlobalInverseTransform() * globalTransform * info.offset;
    }
    else if (boneInfoMap.count(cleanName))
    {
        const auto& info = boneInfoMap.at(cleanName);
        finalBoneMatrices[info.id] = currentModel->GetGlobalInverseTransform() * globalTransform * info.offset;
    }

    for (const auto& child : node->children)
        CalculateBoneTransform(&child, globalTransform);
}