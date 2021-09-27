#pragma once
#include "SoundUtils.h"

FMOD::Sound* CreateSound(FMOD::System* system, std::string audioFile)
{
	std::stringstream ss;
	ss << SOLUTION_DIR << "common\\assets\\audio\\" << audioFile;

	FMOD::Sound* sound = NULL;
	FMOD_RESULT result = system->createSound(ss.str().c_str(), FMOD_LOOP_OFF, 0, &sound);

	if (result != FMOD_OK) {
		fprintf(stderr, "Unable to create sound.");
		return NULL;
	}

	ss.flush();
	return sound;
}

void PlaySound(FMOD::System* system, FMOD::Sound* sound, bool isPaused, FMOD::Channel** channel)
{
	system->playSound(sound, 0, isPaused, channel);
}