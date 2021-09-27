#pragma once
#include <FMOD/fmod.hpp>
#include <fstream>
#include <string>
#include <sstream>

FMOD::Sound* CreateSound(FMOD::System* system, std::string audioFile);