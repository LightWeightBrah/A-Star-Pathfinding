#pragma once
#include <assimp/anim.h>
#include "Model.h"
#include "Bone.h"

class Animator 
{
private:
    float currentTime;
    float deltaTime;
    aiAnimation* currentAnimation;
    Model*       currentModel;

    std::vector<Bone>      bones;
    std::vector<glm::mat4> finalBoneMatrices;

public:
    Animator(Model* model);

    void UpdateAnimation(float deltaTime);
    void PlayAnimation  (aiAnimation* animation);
    void CalculateBoneTransform(const aiNode* node, glm::mat4 parentTransform);

    std::vector<glm::mat4> GetFinalBoneMatrices() { return finalBoneMatrices; }
};