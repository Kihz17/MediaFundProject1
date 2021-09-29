#pragma once
#include <string>
#include <vector>

struct StoryStep
{
	std::string description;
	std::string theme;
	std::vector<std::string> effects;
	bool isLast = 0;
	long waitTime = 3;
};