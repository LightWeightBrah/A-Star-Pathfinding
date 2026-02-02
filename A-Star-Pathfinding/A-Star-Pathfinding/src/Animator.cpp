#include "Animator.h"
#include "AssimpUtilities.h"

Animator::Animator(Model* model)
    : currentModel(model), currentTime(0.0f), currentAnimation(nullptr)
{
    finalBoneMatrices.reserve(100);
    for (int i = 0; i < 100; i++)
        finalBoneMatrices.push_back(glm::mat4(1.0f));
}

void Animator::PlayAnimation(aiAnimation* animation)
{
    currentAnimation = animation;
    currentTime = 0.0f;

    bones.clear();
    for (unsigned int i = 0; i < animation->mNumChannels; i++) 
    {
        auto channel = animation->mChannels[i];
        bones.push_back(Bone(channel->mNodeName.data, 0, channel));
    }
}

void Animator::UpdateAnimation(float deltaTime)
{
    if (!currentAnimation) 
        return;

    float ticksPerSecond = (float)(currentAnimation->mTicksPerSecond != 0 ? currentAnimation->mTicksPerSecond : 25.0f);
    currentTime         += ticksPerSecond * deltaTime;
    currentTime          = fmod(currentTime, (float)currentAnimation->mDuration);

    for (auto& bone : bones)
        bone.Update(currentTime);

    CalculateBoneTransform(currentModel->GetScene()->mRootNode, glm::mat4(1.0f));
}

void Animator::CalculateBoneTransform(const aiNode* node, glm::mat4 parentTransform)
{
    std::string nodeName = node->mName.data;
    glm::mat4 nodeTransform = AssimpUtilities::ConvertAssimpMatrixToGLM(node->mTransformation);

    Bone* bone = nullptr;
    for (auto& b : bones) 
    {
        if (b.GetBoneName() == nodeName) 
        {
            bone = &b;
            break;
        }
    }

    if (bone)
        nodeTransform = bone->GetLocalTransform();

    glm::mat4 globalTransform = parentTransform * nodeTransform;

    auto boneInfoMap = currentModel->GetBoneNameToInfo();
    if (boneInfoMap.find(nodeName) != boneInfoMap.end()) 
    {
        int index = boneInfoMap[nodeName].boneId;
        glm::mat4 offset = boneInfoMap[nodeName].offset;
        finalBoneMatrices[index] = currentModel->GetGlobalInverseTransform() * globalTransform * offset;
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++)
        CalculateBoneTransform(node->mChildren[i], globalTransform);
}