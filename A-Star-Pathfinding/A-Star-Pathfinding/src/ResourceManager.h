#pragma once
#include <map>
#include <string>

#include "Animation.h"
#include <iostream>

enum class AnimationType
{
	IDLE,
	GESTURE,
	RUNNING
};

struct ModelData
{
	Model model;
	std::map<AnimationType, Animation> animations;
};

class ResourceManager
{
private:
	static std::map<std::string, std::shared_ptr<ModelData>> modelsRegistered;

public:
	static void LoadModel(const std::string& name, const std::string& path);
	
	static std::shared_ptr<ModelData> GetModelData(const std::string& name);
};