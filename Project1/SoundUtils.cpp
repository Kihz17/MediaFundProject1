#pragma once
#include "SoundUtils.h"

FMOD::System* _system = NULL;
FMOD_RESULT _result = FMOD_OK;
std::map<std::string, FMOD::Sound*> sounds;

FMOD::Sound* CreateSound(FMOD::System* system, std::string audioFile, std::string soundName)
{
	std::stringstream ss;
	ss << SOLUTION_DIR << "common\\assets\\audio\\" << audioFile;

	FMOD::Sound* sound = NULL;
	_result = system->createSound(ss.str().c_str(), FMOD_LOOP_OFF, 0, &sound);

	if (_result != FMOD_OK) {
		fprintf(stderr, "Unable to create sound.");
		return NULL;
	}

	sounds.insert(std::pair<std::string, FMOD::Sound*>(soundName, sound)); // Store our newly loaded sound into a map indexed by the soundName

	ss.flush();
	return sound;
}