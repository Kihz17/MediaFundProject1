#pragma once
#include <FMOD/fmod.hpp>
#include <fstream>
#include <string>
#include <sstream>
#include <map>

extern FMOD::System* _system;
extern FMOD_RESULT _result;
extern std::map<std::string, FMOD::Sound*> sounds; // Holds all loaded sounds

FMOD::Sound* CreateSound(FMOD::System* system, std::string audioFile, std::string soundName);
