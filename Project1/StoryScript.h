#pragma once
#include <string>
#include <vector>
#include "StoryStep.h";
//Represents the story steps to be told 
class StoryScript
{

public:
	StoryScript();
	~StoryScript();
	StoryStep GetNextLine();
};


